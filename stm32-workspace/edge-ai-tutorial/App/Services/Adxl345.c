#include "Adxl345.h"

#include <stddef.h>

#define ADXL345_I2C_TIMEOUT_MS      10U

#define ADXL345_REG_DEVID           0x00U
#define ADXL345_REG_BW_RATE         0x2CU
#define ADXL345_REG_POWER_CTL       0x2DU
#define ADXL345_REG_DATA_FORMAT     0x31U
#define ADXL345_REG_DATAX0          0x32U
#define ADXL345_REG_FIFO_CTL        0x38U
#define ADXL345_REG_FIFO_STATUS     0x39U

#define ADXL345_DEVID_VALUE         0xE5U
#define ADXL345_BW_RATE_100HZ       0x0AU
#define ADXL345_DATA_FORMAT_FULL_4G 0x09U
#define ADXL345_POWER_CTL_MEASURE   0x08U
#define ADXL345_FIFO_CTL_STREAM     0x80U
#define ADXL345_FIFO_ENTRIES_MASK   0x3FU

#define ADXL345_DATA_LEN            6U

/* Full resolution mode is 3.9 mg/LSB on every range */
#define ADXL345_MILLI_G_NUMERATOR   39
#define ADXL345_MILLI_G_DENOMINATOR 10

static bool adxl345WriteRegister(Adxl345 *self, uint8_t reg, uint8_t value);
static bool adxl345ReadRegisters(Adxl345 *self, uint8_t reg, uint8_t *data, uint16_t length);
static int16_t adxl345ToMilliG(uint8_t low, uint8_t high);

void adxl345Create(Adxl345 *self, I2C_HandleTypeDef *i2c, uint8_t address)
{
    if (self == NULL)
    {
        return;
    }

    self->i2c = i2c;
    self->address = address;
    self->initialized = false;
}

bool adxl345Init(Adxl345 *self)
{
    uint8_t deviceId = 0U;

    if ((self == NULL) || (self->i2c == NULL))
    {
        return false;
    }

    self->initialized = false;

    if (!adxl345ReadRegisters(self, ADXL345_REG_DEVID, &deviceId, 1U))
    {
        return false;
    }

    if (deviceId != ADXL345_DEVID_VALUE)
    {
        return false;
    }

    if (!adxl345WriteRegister(self, ADXL345_REG_BW_RATE, ADXL345_BW_RATE_100HZ) ||
        !adxl345WriteRegister(self, ADXL345_REG_DATA_FORMAT, ADXL345_DATA_FORMAT_FULL_4G) ||
        !adxl345WriteRegister(self, ADXL345_REG_FIFO_CTL, ADXL345_FIFO_CTL_STREAM) ||
        !adxl345WriteRegister(self, ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_MEASURE))
    {
        return false;
    }

    self->initialized = true;

    return true;
}

bool adxl345ReadFifo(Adxl345 *self, Adxl345Sample *samples, uint8_t maxCount, uint8_t *count)
{
    uint8_t data[ADXL345_DATA_LEN] = {0U};
    uint8_t pending = 0U;
    uint8_t i;

    if ((self == NULL) || (samples == NULL) || (count == NULL) || !self->initialized)
    {
        return false;
    }

    *count = 0U;

    if (!adxl345ReadRegisters(self, ADXL345_REG_FIFO_STATUS, &pending, 1U))
    {
        return false;
    }

    pending &= ADXL345_FIFO_ENTRIES_MASK;
    if (pending > maxCount)
    {
        pending = maxCount;
    }

    for (i = 0U; i < pending; i++)
    {
        /* Each read of the six data registers pops one entry. They are read in one transfer
           so the axes belong to the same sample. */
        if (!adxl345ReadRegisters(self, ADXL345_REG_DATAX0, data, ADXL345_DATA_LEN))
        {
            return false;
        }

        samples[i].x = adxl345ToMilliG(data[0], data[1]);
        samples[i].y = adxl345ToMilliG(data[2], data[3]);
        samples[i].z = adxl345ToMilliG(data[4], data[5]);
        *count = (uint8_t)(i + 1U);
    }

    return true;
}

static bool adxl345WriteRegister(Adxl345 *self, uint8_t reg, uint8_t value)
{
    /* HAL expects the 7 bit address left aligned */
    return HAL_I2C_Mem_Write(self->i2c, (uint16_t)(self->address << 1), reg, I2C_MEMADD_SIZE_8BIT,
                             &value, 1U, ADXL345_I2C_TIMEOUT_MS) == HAL_OK;
}

static bool adxl345ReadRegisters(Adxl345 *self, uint8_t reg, uint8_t *data, uint16_t length)
{
    return HAL_I2C_Mem_Read(self->i2c, (uint16_t)(self->address << 1), reg, I2C_MEMADD_SIZE_8BIT,
                            data, length, ADXL345_I2C_TIMEOUT_MS) == HAL_OK;
}

static int16_t adxl345ToMilliG(uint8_t low, uint8_t high)
{
    int16_t raw = (int16_t)(((uint16_t)high << 8) | (uint16_t)low);

    return (int16_t)(((int32_t)raw * ADXL345_MILLI_G_NUMERATOR) / ADXL345_MILLI_G_DENOMINATOR);
}
