#ifndef ADXL345_H
#define ADXL345_H

#include "stm32f7xx_hal.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 7 bit I2C address, selected by the level on the SDO pin */
#define ADXL345_ADDRESS_SDO_LOW     0x53U
#define ADXL345_ADDRESS_SDO_HIGH    0x1DU

/** ADXL345 instance. Treat the members as private and use the adxl345 functions. */
typedef struct
{
    I2C_HandleTypeDef *i2c;
    uint8_t address;
    bool initialized;
} Adxl345;

/** One acceleration sample, each axis in thousandths of g */
typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} Adxl345Sample;

/**
  * @brief  Binds an instance to its bus and address.
  * @param  self instance to set up
  * @param  i2c HAL handle of the I2C bus the sensor is wired to
  * @param  address 7 bit device address, not shifted
  * @return None
  * @note   Does not touch the bus. Call adxl345Init() afterwards.
  */
void adxl345Create(Adxl345 *self, I2C_HandleTypeDef *i2c, uint8_t address);

/**
  * @brief  Detects the sensor and puts it into measurement mode.
  * @param  self instance prepared with adxl345Create()
  * @return true if the device answered with the expected ID and accepted its configuration
  * @note   Configures 100 Hz output data rate, full resolution, +/-4 g.
  *         The I2C peripheral must already be initialized. Safe to call again to retry.
  */
bool adxl345Init(Adxl345 *self);

/**
  * @brief  Reads the latest acceleration sample.
  * @param  self instance initialized with adxl345Init()
  * @param  sample output, acceleration per axis in milli-g
  * @return true on success, false if the sensor is not initialized or the bus transfer failed
  * @note   Blocks for one 6 byte I2C transfer.
  */
bool adxl345Read(Adxl345 *self, Adxl345Sample *sample);

#ifdef __cplusplus
}
#endif

#endif /* ADXL345_H */
