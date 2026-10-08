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
    void streamAccelerometer();
    void sendLine(const char* text);

    Stm32Gpio led1Gpio_;
    Stm32Gpio led2Gpio_;
    Stm32Gpio led3Gpio_;

    Stm32Led led1_;
    Stm32Led led2_;
    Stm32Led led3_;

    /* Longest CSV line is "-16000,-16000,-16000\r\n" */
    static constexpr uint32_t CSV_LINE_MAX_LEN = 24U;

    Adxl345 accelerometer_;
    bool accelerometerReady_;
    uint32_t lastAccelerometerRetryTick_;
    Adxl345Sample samples_[ADXL345_FIFO_DEPTH];
    /* Member rather than local: the USB stack keeps sending from it after run() returns */
    char txBuffer_[ADXL345_FIFO_DEPTH * CSV_LINE_MAX_LEN];
};
