#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  Reads the MCU internal temperature sensor.
  * @param  milliCelsius output, temperature in thousandths of a degree Celsius
  * @return true on a successful conversion, false if the ADC did not complete in time
  * @note   Blocks for the duration of one conversion, roughly 20 us.
  */
bool temperatureSensorRead(int32_t *milliCelsius);

#ifdef __cplusplus
}
#endif

#endif /* TEMPERATURE_SENSOR_H */
