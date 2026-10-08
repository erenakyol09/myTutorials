#include "Application.hpp"
#include "main.h"
#include "i2c.h"
#include "usbd_cdc_if.h"

#include <cstdio>

namespace
{
constexpr uint32_t ACCELEROMETER_REPORT_PERIOD_MS = 1000U;
}

Application::Application()
    : led1Gpio_(LD1_GPIO_Port, LD1_Pin),
      led2Gpio_(LD2_GPIO_Port, LD2_Pin),
      led3Gpio_(LD3_GPIO_Port, LD3_Pin),
      led1_(led1Gpio_),
      led2_(led2Gpio_),
      led3_(led3Gpio_),
      accelerometer_(),
      accelerometerReady_(false),
      lastAccelerometerTick_(0U)
{
}

void Application::init()
{
    led1_.off();
    led2_.off();
    led3_.off();

    adxl345Create(&accelerometer_, &hi2c1, ADXL345_ADDRESS_SDO_LOW);
    accelerometerReady_ = adxl345Init(&accelerometer_);
}

void Application::run()
{
    if ((HAL_GetTick() - lastAccelerometerTick_) >= ACCELEROMETER_REPORT_PERIOD_MS)
    {
        lastAccelerometerTick_ = HAL_GetTick();
        reportAccelerometer();
    }
}

void Application::setLed(bool on)
{
    if (on)
    {
        led2_.on();
    }
    else
    {
        led2_.off();
    }
}

/* Sensor bring-up aid: prints one sample in milli-g over USB CDC and retries the init for as
   long as the sensor does not answer. Best effort, the line is dropped if CDC is busy. */
void Application::reportAccelerometer()
{
    Adxl345Sample sample = {0, 0, 0};
    char line[48];
    int length;

    if (!accelerometerReady_)
    {
        accelerometerReady_ = adxl345Init(&accelerometer_);
    }

    if (!accelerometerReady_)
    {
        length = std::snprintf(line, sizeof(line), "ADXL345 not found\r\n");
    }
    else if (adxl345Read(&accelerometer_, &sample))
    {
        length = std::snprintf(line, sizeof(line), "accel mg x=%d y=%d z=%d\r\n",
                               static_cast<int>(sample.x), static_cast<int>(sample.y),
                               static_cast<int>(sample.z));
    }
    else
    {
        accelerometerReady_ = false;
        length = std::snprintf(line, sizeof(line), "ADXL345 read failed\r\n");
    }

    (void)CDC_Transmit_FS(reinterpret_cast<uint8_t*>(line), static_cast<uint16_t>(length));
}
