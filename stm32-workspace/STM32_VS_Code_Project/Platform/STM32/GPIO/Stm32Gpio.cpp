#include "Stm32Gpio.hpp"

Stm32Gpio::Stm32Gpio(
    GPIO_TypeDef* port,
    uint16_t pin
)
    : port_(port),
      pin_(pin)
{
}

void Stm32Gpio::write(bool state)
{
    HAL_GPIO_WritePin(
        port_,
        pin_,
        state ? GPIO_PIN_SET : GPIO_PIN_RESET
    );
}

void Stm32Gpio::toggle()
{
    HAL_GPIO_TogglePin(
        port_,
        pin_
    );
}