/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwip.h"
#include "usbd_cdc_if.h"
#include "MqttService.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Set to 1 to send a periodic counter over USB CDC, for verifying the port without Ethernet */
#define USB_CDC_HEARTBEAT_ENABLED 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
#if USB_CDC_HEARTBEAT_ENABLED
static void USB_CDC_TestHeartbeat(void);
#endif
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

extern void MX_LWIP_Init(void);
extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_DEVICE (moved before LWIP for diagnostics: if Ethernet/PHY
     init hangs in Error_Handler(), USB CDC still comes up so we can see output) */
  MX_USB_DEVICE_Init();

  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
  mqttServiceInit();

  /* Infinite loop */
#if USB_CDC_HEARTBEAT_ENABLED
  uint32_t lastHeartbeatTick = 0;
#endif
  for(;;)
  {
    MX_LWIP_Process();
    mqttServiceProcess();

#if USB_CDC_HEARTBEAT_ENABLED
    if (HAL_GetTick() - lastHeartbeatTick >= 1000)
    {
      lastHeartbeatTick = HAL_GetTick();
      USB_CDC_TestHeartbeat();
    }
#endif

    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

#if USB_CDC_HEARTBEAT_ENABLED
/**
  * @brief  Sends an incrementing test message over USB CDC, used to verify
  *         the virtual COM port is alive independently of the Ethernet link.
  * @retval None
  */
static void USB_CDC_TestHeartbeat(void)
{
  static uint32_t counter = 0;
  char msg[32];
  int len = snprintf(msg, sizeof(msg), "USB CDC test #%lu\r\n", (unsigned long)counter++);
  CDC_Transmit_FS((uint8_t *)msg, (uint16_t)len);
}
#endif

/* USER CODE END Application */

