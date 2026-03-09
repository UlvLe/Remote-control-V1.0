/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "usart.h"
#include "stdio.h"
#include "joystick.h"
#include "matrix_key.h"
#include "data_uart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for DataGetTask */
osThreadId_t DataGetTaskHandle;
const osThreadAttr_t DataGetTask_attributes = {
  .name = "DataGetTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDataGetTask(void *argument);

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
  /* creation of DataGetTask */
  DataGetTaskHandle = osThreadNew(StartDataGetTask, NULL, &DataGetTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDataGetTask */
/**
  * @brief  Function implementing the DataGetTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDataGetTask */
void StartDataGetTask(void *argument)
{
  /* USER CODE BEGIN StartDataGetTask */
  /* Infinite loop */
  for(;;)
  {
    int16_t LX = 2048 - adc1_values[1];
    if (LX < 0){
      LX += 100;
      if (LX > 0){
        LX = 0;           //ADC修正（-100~0）
      }
    }
    int16_t LY = 2048 - adc1_values[0];
    if (LY < 0){
      LY += 50;
      if (LY > 0){
        LY = 0;           
      }
    }
    if (LY > 0){
      LY -= 120;
      if (LY < 0){
        LY = 0;           //ADC修正（-50~120）
      }
    }
    int16_t RX = 2048 - adc1_values[3];
    if (RX < 0){
      RX += 110;
      if (RX > 0){
        RX = 0;           //ADC修正（-110~0）
      }
    }
    int16_t RY = 2048 - adc1_values[2];
    if (RY < 0){
      RY += 160;
      if (RY > 0){
        RY = 0;           //ADC修正（-160~0）
      }
    }
    uint16_t joystickbutton_state = ReadJoystickbutton();
    uint16_t key_state = ScanMatrixKey();
    //printf("%d,%d,%d,%d,%d,%d\n",LX,LY,RX,RY,joystickbutton_state,key_state);

    uint8_t packet[14]; // 14 字节

    packet[0] = 0xAA;      //包头

    packet[1] = (LX >> 8) & 0xFF;
    packet[2] = LX & 0xFF;        //左摇杆X值

    packet[3] = (LY >> 8) & 0xFF;
    packet[4] = LY & 0xFF;        //左摇杆Y值

    packet[5] = (RX >> 8) & 0xFF;
    packet[6] = RX & 0xFF;        //右摇杆X值

    packet[7] = (RY >> 8) & 0xFF;
    packet[8] = RY & 0xFF;        //右摇杆Y值

    packet[9] = joystickbutton_state;    //摇杆按键状态

    packet[10] = (key_state >> 8) & 0xFF;
    packet[11] = key_state & 0xFF;        //矩阵键盘数据

    uint8_t crc = CRC8(&packet[1], 11);
    packet[12] = crc;            // CRC校验：从 packet[1] 到 packet[11]，共 11 字节

    packet[13] = 0xBB;      //包尾

/*
    uint8_t packet[14]; // 15 字节
    packet[0] = 0xAA;      
    packet[1] = 0x12;     
    packet[2] = 0x34;
    packet[3] = 0x56;     
    packet[4] = 0x78;
    packet[5] = 0x9A; 
    packet[6] = 0xBC;
    packet[7] = 0xDE;   
    packet[8] = 0xF0;
    packet[9] = 0x11;    
    packet[10] = 0x22;   
    packet[11] = 0x33;   
    uint8_t crc = CRC8(&packet[1], 11);
    packet[12] = crc;          
    packet[13] = 0xBB;   
*/
/*
    uint8_t settingpacket[4];
    settingpacket[0] = 0xC0;
    settingpacket[1] = 0x03;
    settingpacket[2] = 0x01;
    settingpacket[3] = 0xE5;
*/
    HAL_UART_Transmit_DMA(&huart1, packet, 14);
    //printf("\n");
    osDelay(20);

  }
  /* USER CODE END StartDataGetTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

