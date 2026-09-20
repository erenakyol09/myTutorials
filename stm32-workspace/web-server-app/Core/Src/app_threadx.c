/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include <string.h>
#include "ApplicationWrapper.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint32_t test_val = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
#define AZURE_TASK_STACK_SIZE    1024U    /* bytes */
#define AZURE_TASK_PRIORITY      10U

static TX_THREAD azureTask;
static void azureTaskEntry(ULONG thread_input);
static void *azureTaskStackPtr = TX_NULL;

/* Fallback static stack buffer used when byte pool allocation fails */
static uint8_t azureTaskStackStatic[AZURE_TASK_STACK_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static void azureTaskEntry(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  {
      UINT status;

      /* Attempt to allocate stack memory for the Azure task from the provided byte pool */
      status = tx_byte_allocate(byte_pool, &azureTaskStackPtr, AZURE_TASK_STACK_SIZE, TX_NO_WAIT);
      if (status != TX_SUCCESS)
      {
          if (status == TX_NO_MEMORY)
          {
              /* Use fallback static buffer when the byte pool is out of memory */
              azureTaskStackPtr = azureTaskStackStatic;
              /* Optional: clear fallback stack for deterministic startup */
              memset(azureTaskStackPtr, 0, AZURE_TASK_STACK_SIZE);
          }
          else
          {
              /* For other errors, propagate failure */
              ret = status;
              return ret;
          }
      }

      /* Create and auto-start the Azure task.
         The task performs periodic work; adjust sleep period if tick rate differs. */
      status = tx_thread_create(&azureTask,
                                "AzureTask",
                                azureTaskEntry,
                                0,
                                azureTaskStackPtr,
                                AZURE_TASK_STACK_SIZE,
                                AZURE_TASK_PRIORITY,
                                AZURE_TASK_PRIORITY,
                                TX_NO_TIME_SLICE,
                                TX_AUTO_START);
      if (status != TX_SUCCESS)
      {
          ret = status;
          return ret;
      }
  }
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
static void azureTaskEntry(ULONG thread_input)
{
    (void)thread_input;

    App_Init();

    while (1)
    {
        App_Run();
        test_val++;
        tx_thread_sleep(100); /* one second*/
    }
}
/* USER CODE END 1 */
