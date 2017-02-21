/**
  ******************************************************************************
  * @file    logger.c 
  * @author  CL
  * @version V1.0.0
  * @date    010-July-2014
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "logger.h"
#include <string.h>
#include <stdio.h>

/** @addtogroup X-CUBE-BLE1_Applications
 *  @{
 */

/** @addtogroup Virtual_COM_Port
 *  @brief
 *  @{
 */

/** @defgroup LOGGER
 * @{
 */

/** @defgroup LOGGER_Private_Defines 
 * @{
 */
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
/**
  * @}
  */

/*
#undef putchar

int putchar(int c)
{
  int timeout = 15;
  uint8_t c8 = c;
  
  
  if (HAL_UART_Transmit(&huart2, &c8, 1, timeout) != HAL_OK) {
    //return EOF;
  }

  return c;
}

void __io_putchar(char c)
{
  putchar(c);
}
*/

/** @defgroup LOGGER_Exported_Functions 
 * @{
 */
/** 
* @brief  USART2 init function.
* @param  None
* @return None
*/
void vcom_init(void)
// FIXME: replace hardcoded pin configuration with defines
{
  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  //huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  //huart2.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  //huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart2);

}

/** 
* @brief Print log function.
* @param String to print
* @return None
*/
void log_print(char* string)
{
  char* p = string;
  int len = 0;
  
  while (*p++) {
    len++;
  }
  
  if (len > 0) {
    log_string(string, len);
  }
}

/** 
* @brief Print a string.
* @param String to print
* @param Length of the string
* @return None
*/
void log_string(char* string, uint32_t len)
{
  HAL_StatusTypeDef status;
  int timeout = 300;
  
  status = HAL_UART_Transmit(&huart2, (uint8_t*)string, len, timeout);
  
  printf("status tx=%d\n", status);
}

/** 
* @brief Print data of a SPI transaction.
* @param Read/Write transaction type
* @param First buffer
* @param Second buffer
* @param Size of the first buffer
* @param Size of the second buffer
* @return None
*/
void log_spi_data(rw_t rw, uint8_t* buffer1, uint8_t* buffer2, uint8_t size1, uint8_t size2)
{ 
  char op = (rw == READ) ? 'R' : 'W';
  int i;
  char str[80];
  
  snprintf(str, sizeof(str), "%d %c: {", HAL_GetTick(), op);
  log_string(str, strlen(str));
  
  for (i=0; i<size1; i++) {
    snprintf(str, sizeof(str), "0x%02x,", buffer1[i]);
    log_string(str, strlen(str));
  }

  for (i=0; i<size2; i++) {
    snprintf(str, sizeof(str), "0x%02x,", buffer2[i]);
    log_string(str, strlen(str));
  }
  
  snprintf(str, sizeof(str), "};\n");
  log_string(str, strlen(str));
}

/** 
* @brief  Callback managing an UART rx.
* @param  UART handle pointer
* @return None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  printf("RX Callback\n");  
}

/** 
* @brief  UART Error Callback.
* @param  UART handle
* @return None
*/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  printf("UART Error Callback\n");  
}

/** 
* @brief  MspInit of the UART (it is called by the Cube HAL API at UART init).
* @param  UART handle pointer
* @return None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART2)
  {
    /* Peripheral clock enable */
    __USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* Peripheral interrupt init*/
    // FIXME: check IRQ priority to avoid deadlock
    //HAL_NVIC_SetPriority(USART2_IRQn, 4, 0);
    //HAL_NVIC_EnableIRQ(USART2_IRQn);
    
  }
}
/**
 * @}
 */
 
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
