#include "GestureClassifier.h"

#include "stm32f7xx_hal.h"

#include <stddef.h>
#include <string.h>

/* Must match SCALE_MILLI_G in ml/train.py, the model only knows samples preprocessed the way
   that script does it: scaled, then with the window mean removed */
#define GESTURE_SCALE_MILLI_G       4000.0f

#define GESTURE_PERCENT             100.0f
#define GESTURE_MICROS_PER_SECOND   1000000U

/* Key that unlocks write access to the cycle counter registers on Cortex-M7 */
#define GESTURE_DWT_UNLOCK_KEY      0xC5ACCE55U

/* Output order of the network, must match ml/models/labels.txt */
static const char *const GESTURE_LABEL_NAMES[] = {"circle", "idle", "shake", "updown"};

_Static_assert((sizeof(GESTURE_LABEL_NAMES) / sizeof(GESTURE_LABEL_NAMES[0])) == GESTURE_LABEL_COUNT,
               "label names do not match the number of network outputs");

static void gestureClassifierStartCycleCounter(void);
static void gestureClassifierRemoveMean(float *window);
static bool gestureClassifierRun(GestureClassifier *self, GestureResult *result);

bool gestureClassifierInit(GestureClassifier *self)
{
    ai_handle activations[AI_NETWORK_DATA_ACTIVATIONS_COUNT] = {NULL};
    ai_error error;

    if (self == NULL)
    {
        return false;
    }

    self->initialized = false;
    self->network = AI_HANDLE_NULL;
    self->head = 0U;
    self->filled = 0U;
    self->sinceLastRun = 0U;
    (void)memset(self->window, 0, sizeof(self->window));

    activations[0] = AI_HANDLE_PTR(self->activations);
    error = ai_network_create_and_init(&self->network, activations, NULL);
    if (error.type != AI_ERROR_NONE)
    {
        return false;
    }

    gestureClassifierStartCycleCounter();
    self->initialized = true;

    return true;
}

bool gestureClassifierPush(GestureClassifier *self, int16_t x, int16_t y, int16_t z,
                           GestureResult *result)
{
    float *slot;

    if ((self == NULL) || (result == NULL) || !self->initialized)
    {
        return false;
    }

    slot = &self->window[(size_t)self->head * GESTURE_AXIS_COUNT];
    slot[0] = (float)x / GESTURE_SCALE_MILLI_G;
    slot[1] = (float)y / GESTURE_SCALE_MILLI_G;
    slot[2] = (float)z / GESTURE_SCALE_MILLI_G;
    self->head = (uint16_t)((self->head + 1U) % GESTURE_WINDOW_SAMPLES);

    if (self->filled < GESTURE_WINDOW_SAMPLES)
    {
        self->filled++;
        if (self->filled < GESTURE_WINDOW_SAMPLES)
        {
            return false;
        }
        /* First full window: classify it right away */
        self->sinceLastRun = GESTURE_STRIDE_SAMPLES;
    }
    else
    {
        self->sinceLastRun++;
    }

    if (self->sinceLastRun < GESTURE_STRIDE_SAMPLES)
    {
        return false;
    }

    self->sinceLastRun = 0U;

    return gestureClassifierRun(self, result);
}

const char *gestureClassifierLabelName(uint8_t label)
{
    if (label >= GESTURE_LABEL_COUNT)
    {
        return "unknown";
    }

    return GESTURE_LABEL_NAMES[label];
}

static void gestureClassifierStartCycleCounter(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->LAR = GESTURE_DWT_UNLOCK_KEY;
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/**
  * @brief  Subtracts from each axis its mean over the window.
  * @param  window interleaved x, y, z samples of one full window, modified in place
  * @return None
  * @note   Removes gravity so the model sees the motion and not the angle the sensor is held at.
  *         Must match remove_window_mean() in ml/train.py.
  */
static void gestureClassifierRemoveMean(float *window)
{
    uint16_t axis;
    uint16_t sample;

    for (axis = 0U; axis < GESTURE_AXIS_COUNT; axis++)
    {
        float sum = 0.0f;
        float mean;

        for (sample = 0U; sample < GESTURE_WINDOW_SAMPLES; sample++)
        {
            sum += window[((size_t)sample * GESTURE_AXIS_COUNT) + axis];
        }

        mean = sum / (float)GESTURE_WINDOW_SAMPLES;

        for (sample = 0U; sample < GESTURE_WINDOW_SAMPLES; sample++)
        {
            window[((size_t)sample * GESTURE_AXIS_COUNT) + axis] -= mean;
        }
    }
}

/**
  * @brief  Copies the window into the network input, runs it and picks the best output.
  * @param  self initialized instance holding a full window
  * @param  result output, classification of the window
  * @return true if the network ran, false if it reported an error
  */
static bool gestureClassifierRun(GestureClassifier *self, GestureResult *result)
{
    ai_buffer *input = ai_network_inputs_get(self->network, NULL);
    ai_buffer *output = ai_network_outputs_get(self->network, NULL);
    float *inputData = (float *)input[0].data;
    const float *outputData = (const float *)output[0].data;
    size_t oldest = (size_t)self->head * GESTURE_AXIS_COUNT;
    size_t tail = sizeof(self->window) / sizeof(self->window[0]) - oldest;
    uint32_t startCycles;
    uint32_t elapsedCycles;
    uint8_t best = 0U;
    uint8_t i;

    /* head points at the oldest sample, so unrolling the ring from there puts the window in
       chronological order */
    (void)memcpy(inputData, &self->window[oldest], tail * sizeof(float));
    (void)memcpy(&inputData[tail], self->window, oldest * sizeof(float));
    gestureClassifierRemoveMean(inputData);

    startCycles = DWT->CYCCNT;
    if (ai_network_run(self->network, input, output) != 1)
    {
        return false;
    }
    elapsedCycles = DWT->CYCCNT - startCycles;

    for (i = 1U; i < GESTURE_LABEL_COUNT; i++)
    {
        if (outputData[i] > outputData[best])
        {
            best = i;
        }
    }

    result->label = best;
    result->confidence = (uint8_t)((outputData[best] * GESTURE_PERCENT) + 0.5f);
    result->inferenceMicros = (uint32_t)(((uint64_t)elapsedCycles * GESTURE_MICROS_PER_SECOND) /
                                         SystemCoreClock);

    return true;
}
