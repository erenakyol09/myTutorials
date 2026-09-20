#include "Application.hpp"
#include "main.h"



Application::Application()
    : led1Gpio_(LD1_GPIO_Port, LD1_Pin),
      led2Gpio_(LD2_GPIO_Port, LD2_Pin),
      led3Gpio_(LD3_GPIO_Port, LD3_Pin),
      led1_(led1Gpio_),
      led2_(led2Gpio_),
      led3_(led3Gpio_)
{
}

void Application::init()
{
    led1_.on();
    led2_.off();
    led3_.on();
}

void Application::run()
{
}
