#ifndef APPLICATION_WRAPPER_H
#define APPLICATION_WRAPPER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void App_Init(void);
void App_Run(void);

/**
  * @brief  Drives the MQTT controlled user LED (LD2).
  * @param  on true turns the LED on, false turns it off
  * @return None
  */
void App_SetLed(bool on);

#ifdef __cplusplus
}
#endif

#endif