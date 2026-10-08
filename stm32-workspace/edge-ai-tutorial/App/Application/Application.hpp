#pragma once

#include "ILed.hpp"
#include "Stm32Led.hpp"
#include "Stm32Gpio.hpp"
#include "Adxl345.h"

class Application
{
public:
    Application();

    void init();
    void run();
    void setLed(bool on);

private:
    void reportAccelerometer();

    Stm32Gpio led1Gpio_;
    Stm32Gpio led2Gpio_;
    Stm32Gpio led3Gpio_;

    Stm32Led led1_;
    Stm32Led led2_;
    Stm32Led led3_;

    Adxl345 accelerometer_;
    bool accelerometerReady_;
    uint32_t lastAccelerometerTick_;
};
