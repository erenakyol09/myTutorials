#include "ApplicationWrapper.h"
#include "Application.hpp"

static Application application;

extern "C"
{

void App_Init(void)
{
    application.init();
}

void App_Run(void)
{
    application.run();
}

}