#pragma once

#include "ILed.hpp"
#include "IGpio.hpp"

class Stm32Led : public ILed
{
public:
    explicit Stm32Led(IGpio& gpio);

    void on() override;
    void off() override;
    void toggle() override;

private:
    IGpio& gpio_;
};