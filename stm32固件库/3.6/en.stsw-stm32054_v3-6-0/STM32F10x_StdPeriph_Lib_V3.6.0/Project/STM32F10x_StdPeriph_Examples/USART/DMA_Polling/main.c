/**
  ******************************************************************************
  * @file    USART/DMA_Polling/main.c 
  * @author  MCD Application Team
  * @version V3.6.0
  * @date    20-September-2021
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2011 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "platform_config.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_DMA_Polling
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
uint8_t TxBuffer1[] = "USART DMA Polling: USARTy -> USARTz using DMA";
uint8_t TxBuffer2[] = "USART DMA Polling: USARTz -> USARTy using DMA";
uint8_t RxBuffer1[TxBufferSize2];
uint8_t RxBuffer2[TxBufferSize1];
volatile TestStatus TransferStatus1 = FAILED;
volatile TestStatus TransferStatus2 = FAILED; 

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void DMA_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
  /* System Clocks Configuration */
  RCC_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* Configure the DMA */
  DMA_Configuration();

/* USARTy and USARTz configuration ------------------------------------------------------*/
  /* USARTy and USARTz configured as follow:
        - BaudRate = 230400 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 230400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure USARTy */
  USART_Init(USARTy, &USART_InitStructure);
  /* Configure USARTz */
  USART_Init(USARTz, &USART_InitStructure);

  /* Enable USARTy DMA Rx and TX request */
  USART_DMACmd(USARTy, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);
  /* Enable USARTz DMA Rx and TX request */
  USART_DMACmd(USARTz, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);

  /* Enable USARTy TX DMA1 Channel */
  DMA_Cmd(USARTy_Tx_DMA_Channel, ENABLE);
  /* Enable USARTy RX DMA1 Channel */
  DMA_Cmd(USARTy_Rx_DMA_Channel, ENABLE);

  /* Enable USARTz TX DMA1 Channel */
  DMA_Cmd(USARTz_Tx_DMA_Channel, ENABLE);
  /* Enable USARTz RX DMA1 Channel */
  DMA_Cmd(USARTz_Rx_DMA_Channel, ENABLE);

  /* Enable the USARTy */
  USART_Cmd(USARTy, ENABLE);
  /* Enable the USARTz */
  USART_Cmd(USARTz, ENABLE);

  /* Wait until USARTy TX DMA1 Channel Transfer Complete */
  while (DMA_GetFlagStatus(USARTy_Tx_DMA_FLAG) == RESET)
  {
  }
  /* Wait until USARTy RX DMA1 Channel Transfer Complete */
  while (DMA_GetFlagStatus(USARTy_Rx_DMA_FLAG) == RESET)
  {
  }

  /* Wait until USARTz TX DMA1 Channel Transfer Complete */
  while (DMA_GetFlagStatus(USARTz_Tx_DMA_FLAG) == RESET)
  {
  }
  /* Wait until USARTz RX DMA1 Channel Transfer Complete */
  while (DMA_GetFlagStatus(USARTz_Rx_DMA_FLAG) == RESET)
  {
  }

  /* Check the received data with the send ones */
  TransferStatus1 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);
  /* TransferStatus1 = PASSED, if the data transmitted from USARTz and  
     received by USARTy are the same */
  /* TransferStatus1 = FAILED, if the data transmitted from USARTz and 
     received by USARTy are different */
  TransferStatus2 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
  /* TransferStatus2 = PASSED, if the data transmitted from USARTy and  
     received by USARTz are the same */
  /* TransferStatus2 = FAILED, if the data transmitted from USARTy and 
     received by USARTz are different */

  while (1)
  {
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{  
  /* DMA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK | USARTz_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

#ifndef USE_STM3210C_EVAL
  /* Enable USARTy Clock */
  RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE); 
#else
  /* Enable USARTy Clock */
  RCC_APB1PeriphClockCmd(USARTy_CLK, ENABLE); 
#endif
  /* Enable USARTz Clock */
  RCC_APB1PeriphClockCmd(USARTz_CLK, ENABLE);  
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

#ifdef USE_STM3210C_EVAL
  /* Enable the USART3 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
  
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);  
#elif defined(USE_STM3210B_EVAL) || defined(USE_STM32100B_EVAL)
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#endif

  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USARTy_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);
  
  /* Configure USARTz Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);  
  
  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USARTy_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

  /* Configure USARTz Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure); 
}

/**
  * @brief  Configures the DMA.
  * @param  None
  * @retval None
  */
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* USARTy TX DMA1 Channel (triggered by USARTy Tx event) Config */
  DMA_DeInit(USARTy_Tx_DMA_Channel);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = USARTy_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(USARTy_Tx_DMA_Channel, &DMA_InitStructure);

  /* USARTy RX DMA1 Channel (triggered by USARTy Rx event) Config */
  DMA_DeInit(USARTy_Rx_DMA_Channel);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = USARTy_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = TxBufferSize2;
  DMA_Init(USARTy_Rx_DMA_Channel, &DMA_InitStructure);
  
  /* USARTz TX DMA1 Channel (triggered by USARTz Tx event) Config */
  DMA_DeInit(USARTz_Tx_DMA_Channel);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = USARTz_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer2;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = TxBufferSize2;  
  DMA_Init(USARTz_Tx_DMA_Channel, &DMA_InitStructure);
  
  /* USARTz RX DMA1 Channel (triggered by USARTz Rx event) Config */
  DMA_DeInit(USARTz_Rx_DMA_Channel);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = USARTz_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer2;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
  DMA_Init(USARTz_Rx_DMA_Channel, &DMA_InitStructure);  
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

