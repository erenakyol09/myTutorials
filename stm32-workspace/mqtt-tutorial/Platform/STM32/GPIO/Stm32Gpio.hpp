#pragma once

#include "IGpio.hpp"

#include "stm32f7xx_hal.h"

class Stm32Gpio : public IGpio
{
public:
    Stm32Gpio(
        GPIO_TypeDef* port,
        uint16_t pin
    );

    void write(bool state) override;
    void toggle() override;

private:
    GPIO_TypeDef* port_;
    uint16_t pin_;
};