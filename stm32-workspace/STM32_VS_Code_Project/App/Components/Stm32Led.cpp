#include "Stm32Led.hpp"

Stm32Led::Stm32Led(IGpio& gpio)
    : gpio_(gpio)
{
}

void Stm32Led::on()
{
    gpio_.write(true);
}

void Stm32Led::off()
{
    gpio_.write(false);
}

void Stm32Led::toggle()
{
    gpio_.toggle();
}