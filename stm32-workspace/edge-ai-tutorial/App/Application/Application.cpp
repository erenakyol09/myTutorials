#include "Application.hpp"
#include "main.h"
#include "i2c.h"
#include "usbd_cdc_if.h"

#include <cstdio>

extern "C"
{
extern USBD_HandleTypeDef hUsbDeviceFS;
}

namespace
{
constexpr uint32_t ACCELEROMETER_RETRY_PERIOD_MS = 1000U;

/* True when a host has configured the port and the previous transfer has completed */
bool usbCdcReady()
{
    const USBD_CDC_HandleTypeDef* cdc =
        static_cast<const USBD_CDC_HandleTypeDef*>(hUsbDeviceFS.pClassData);

    return (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) && (cdc != nullptr) &&
           (cdc->TxState == 0U);
}
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
      lastAccelerometerRetryTick_(0U),
      samples_(),
      txBuffer_()
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
    streamAccelerometer();
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

/* Streams every buffered accelerometer sample over USB CDC as a "x,y,z" CSV line in milli-g.
   Lines starting with '#' are status messages. Nothing is drained while the port is busy or
   closed, the sensor FIFO absorbs up to 320 ms and then drops the oldest samples. */
void Application::streamAccelerometer()
{
    uint8_t count = 0U;
    uint32_t length = 0U;

    if (!usbCdcReady())
    {
        return;
    }

    if (!accelerometerReady_)
    {
        if ((HAL_GetTick() - lastAccelerometerRetryTick_) < ACCELEROMETER_RETRY_PERIOD_MS)
        {
            return;
        }

        lastAccelerometerRetryTick_ = HAL_GetTick();
        accelerometerReady_ = adxl345Init(&accelerometer_);
        if (!accelerometerReady_)
        {
            sendLine("# ADXL345 not found\r\n");
        }
        return;
    }

    if (!adxl345ReadFifo(&accelerometer_, samples_, ADXL345_FIFO_DEPTH, &count))
    {
        accelerometerReady_ = false;
        sendLine("# ADXL345 read failed\r\n");
        return;
    }

    for (uint8_t i = 0U; i < count; i++)
    {
        const int written = std::snprintf(&txBuffer_[length], sizeof(txBuffer_) - length,
                                          "%d,%d,%d\r\n",
                                          static_cast<int>(samples_[i].x),
                                          static_cast<int>(samples_[i].y),
                                          static_cast<int>(samples_[i].z));

        if ((written <= 0) || (static_cast<uint32_t>(written) >= (sizeof(txBuffer_) - length)))
        {
            break;
        }

        length += static_cast<uint32_t>(written);
    }

    if (length > 0U)
    {
        (void)CDC_Transmit_FS(reinterpret_cast<uint8_t*>(txBuffer_), static_cast<uint16_t>(length));
    }
}

void Application::sendLine(const char* text)
{
    const int length = std::snprintf(txBuffer_, sizeof(txBuffer_), "%s", text);

    if (length > 0)
    {
        (void)CDC_Transmit_FS(reinterpret_cast<uint8_t*>(txBuffer_), static_cast<uint16_t>(length));
    }
}
