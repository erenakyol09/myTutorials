#pragma once

#include "ILed.hpp"
#include "Stm32Led.hpp"
#include "Stm32Gpio.hpp"

class Application
{
public:
    Application();

    void init();
    void run();

private:
    Stm32Gpio led1Gpio_;
    Stm32Gpio led2Gpio_;
    Stm32Gpio led3Gpio_;

    Stm32Led led1_;
    Stm32Led led2_;
    Stm32Led led3_;
};