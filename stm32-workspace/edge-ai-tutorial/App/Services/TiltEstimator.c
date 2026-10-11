#include "TiltEstimator.h"

#include <math.h>
#include <stddef.h>

#define TILT_ONE_G_MILLI_G      1000.0f
#define TILT_FULL_SCALE         1000.0f
#define TILT_RAD_TO_DEG         57.29578f

static int16_t tiltRound(float value);

void tiltEstimatorCreate(TiltEstimator *self, float smoothing)
{
    if (self == NULL)
    {
        return;
    }

    self->smoothing = smoothing;
    self->gravityX = 0.0f;
    self->gravityY = 0.0f;
    self->gravityZ = 0.0f;
    self->primed = false;
}

void tiltEstimatorUpdate(TiltEstimator *self, int16_t x, int16_t y, int16_t z, TiltState *state)
{
    float sampleX = (float)x;
    float sampleY = (float)y;
    float sampleZ = (float)z;
    float tiltX;
    float tiltY;
    float tiltZ;
    float tiltLength;

    if ((self == NULL) || (state == NULL))
    {
        return;
    }

    if (!self->primed)
    {
        /* Start from the first sample so the output does not ramp up from zero */
        self->gravityX = sampleX;
        self->gravityY = sampleY;
        self->gravityZ = sampleZ;
        self->primed = true;
    }
    else
    {
        self->gravityX += self->smoothing * (sampleX - self->gravityX);
        self->gravityY += self->smoothing * (sampleY - self->gravityY);
        self->gravityZ += self->smoothing * (sampleZ - self->gravityZ);
    }

    tiltX = -self->gravityX / TILT_ONE_G_MILLI_G;
    tiltY = -self->gravityY / TILT_ONE_G_MILLI_G;
    tiltLength = sqrtf((tiltX * tiltX) + (tiltY * tiltY));
    if (tiltLength > 1.0f)
    {
        tiltX /= tiltLength;
        tiltY /= tiltLength;
    }

    tiltZ = fminf(fmaxf(self->gravityZ / TILT_ONE_G_MILLI_G, -1.0f), 1.0f);

    state->magnitude = tiltRound(sqrtf((sampleX * sampleX) + (sampleY * sampleY) + (sampleZ * sampleZ)));
    state->tiltX = tiltRound(tiltX * TILT_FULL_SCALE);
    state->tiltY = tiltRound(tiltY * TILT_FULL_SCALE);
    state->tiltZ = tiltRound(tiltZ * TILT_FULL_SCALE);
    state->pitch = tiltRound(atan2f(-self->gravityX,
                                    sqrtf((self->gravityY * self->gravityY) +
                                          (self->gravityZ * self->gravityZ))) * TILT_RAD_TO_DEG);
    state->roll = tiltRound(atan2f(self->gravityY, self->gravityZ) * TILT_RAD_TO_DEG);
}

static int16_t tiltRound(float value)
{
    return (int16_t)lroundf(value);
}
