#ifndef GESTURE_CLASSIFIER_H
#define GESTURE_CLASSIFIER_H

#include "network.h"
#include "network_data.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Samples per classified window and axes per sample, fixed by the trained model */
#define GESTURE_WINDOW_SAMPLES      AI_NETWORK_IN_1_HEIGHT
#define GESTURE_AXIS_COUNT          AI_NETWORK_IN_1_CHANNEL
#define GESTURE_LABEL_COUNT         AI_NETWORK_OUT_1_SIZE

/* Samples between two classifications: 20 samples at 100 Hz gives 5 results per second */
#define GESTURE_STRIDE_SAMPLES      20U

/** Gesture classifier instance. Treat the members as private and use the gestureClassifier functions. */
typedef struct
{
    ai_handle network;
    /* Working memory of the network, it also holds the input and output tensors */
    AI_ALIGNED(4) uint8_t activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
    /* Ring buffer of the most recent scaled samples, interleaved x, y, z */
    float window[GESTURE_WINDOW_SAMPLES * GESTURE_AXIS_COUNT];
    uint16_t head;
    uint16_t filled;
    uint16_t sinceLastRun;
    bool initialized;
} GestureClassifier;

/** Outcome of one classification */
typedef struct
{
    uint8_t label;              /* index of the most likely gesture, see gestureClassifierLabelName() */
    uint8_t confidence;         /* probability of that gesture, percent */
    uint32_t inferenceMicros;   /* time the network took to run */
} GestureResult;

/**
  * @brief  Creates the network and clears the sample window.
  * @param  self instance to set up
  * @return true if the network was created and initialized
  * @note   The generated network is a single global object, so only one instance may exist.
  */
bool gestureClassifierInit(GestureClassifier *self);

/**
  * @brief  Adds one acceleration sample and classifies the window when one is due.
  * @param  self instance initialized with gestureClassifierInit()
  * @param  x acceleration on the X axis, milli-g
  * @param  y acceleration on the Y axis, milli-g
  * @param  z acceleration on the Z axis, milli-g
  * @param  result output, written only when the function returns true
  * @return true if this sample completed a window and result holds a new classification
  * @note   Must be fed every 100 Hz sample in order, the model was trained on that rate.
  *         Runs the network once every GESTURE_STRIDE_SAMPLES calls after the first full
  *         window, the other calls only store the sample.
  */
bool gestureClassifierPush(GestureClassifier *self, int16_t x, int16_t y, int16_t z,
                           GestureResult *result);

/**
  * @brief  Returns the name of a gesture.
  * @param  label index from GestureResult
  * @return Name as used when recording the training data, "unknown" if out of range
  */
const char *gestureClassifierLabelName(uint8_t label);

#ifdef __cplusplus
}
#endif

#endif /* GESTURE_CLASSIFIER_H */
