#ifndef TILT_ESTIMATOR_H
#define TILT_ESTIMATOR_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Tilt estimator instance. Treat the members as private and use the tiltEstimator functions. */
typedef struct
{
    float smoothing;
    float gravityX;
    float gravityY;
    float gravityZ;
    bool primed;
} TiltEstimator;

/** Orientation derived from one acceleration sample */
typedef struct
{
    int16_t magnitude;  /* length of the unfiltered acceleration vector, milli-g */
    int16_t tiltX;      /* position toward the lowered side, thousandths of full tilt */
    int16_t tiltY;
    int16_t pitch;      /* degrees, -90..90 */
    int16_t roll;       /* degrees, -180..180 */
} TiltState;

/**
  * @brief  Prepares an instance.
  * @param  self instance to set up
  * @param  smoothing weight of each new sample in the gravity low-pass filter, 0..1.
  *         Smaller is steadier but slower, 1 disables the filter.
  * @return None
  */
void tiltEstimatorCreate(TiltEstimator *self, float smoothing);

/**
  * @brief  Feeds one acceleration sample and computes the current orientation.
  * @param  self instance prepared with tiltEstimatorCreate()
  * @param  x acceleration on the X axis, milli-g
  * @param  y acceleration on the Y axis, milli-g
  * @param  z acceleration on the Z axis, milli-g
  * @param  state output, orientation after this sample
  * @return None
  * @note   Must be called once per sample, the filter time constant depends on the sample rate.
  *         tiltX and tiltY form a point inside the unit circle that moves toward the side of
  *         the sensor that is tilted down, like a ball on a plate. An axis reads negative when
  *         it points down, so they have the opposite sign of the filtered acceleration.
  */
void tiltEstimatorUpdate(TiltEstimator *self, int16_t x, int16_t y, int16_t z, TiltState *state);

#ifdef __cplusplus
}
#endif

#endif /* TILT_ESTIMATOR_H */
