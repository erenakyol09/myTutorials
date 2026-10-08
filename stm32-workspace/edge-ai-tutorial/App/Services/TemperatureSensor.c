#include "TemperatureSensor.h"

#include "adc.h"

/* Factory calibration values measured at 3.3 V, see RM0410 section 15.10 */
#define TEMP_CAL1_ADDR      ((const volatile uint16_t *)0x1FF0F44CU)
#define TEMP_CAL2_ADDR      ((const volatile uint16_t *)0x1FF0F44EU)
#define TEMP_CAL1_MILLI_C   30000
#define TEMP_CAL2_MILLI_C   110000

#define TEMP_ADC_TIMEOUT_MS 10U

bool temperatureSensorRead(int32_t *milliCelsius)
{
    int32_t cal1 = (int32_t)(*TEMP_CAL1_ADDR);
    int32_t cal2 = (int32_t)(*TEMP_CAL2_ADDR);
    int32_t raw;

    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
        return false;
    }

    if (HAL_ADC_PollForConversion(&hadc1, TEMP_ADC_TIMEOUT_MS) != HAL_OK)
    {
        (void)HAL_ADC_Stop(&hadc1);
        return false;
    }

    raw = (int32_t)HAL_ADC_GetValue(&hadc1);
    (void)HAL_ADC_Stop(&hadc1);

    /* Linear interpolation between the two calibration points. The largest intermediate value is
       4095 * 80000, which still fits in int32_t. */
    *milliCelsius = (((raw - cal1) * (TEMP_CAL2_MILLI_C - TEMP_CAL1_MILLI_C)) / (cal2 - cal1))
                    + TEMP_CAL1_MILLI_C;

    return true;
}
