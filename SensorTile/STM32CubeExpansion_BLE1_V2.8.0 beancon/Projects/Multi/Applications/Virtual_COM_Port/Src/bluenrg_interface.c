/**
  ******************************************************************************
  * @file    bluenrg_interface.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   This file provides code for the BlueNRG Expansion Board driver
  *          based on STM32Cube HAL for STM32 Nucleo boards.
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
#include "bluenrg_interface.h"

#include "debug.h"
#include "ble_status.h"
#include "hci.h"
#include "stm32_bluenrg_ble.h"

extern SPI_HandleTypeDef SpiHandle;
extern UART_HandleTypeDef UartHandle;


// FIXME: we don't need this because in this project we're using polling
// for SPI transactions and we rely on the weak version provided by Cube HAL
#if 0
/**
 * @brief  EXTI line detection callback.
 * @param  uint16_t GPIO_Pin Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  tHciDataPacket * hciReadPacket = NULL;
  uint8_t data_len;
  /* 
   * No need to call Clear_SPI_EXTI_Flag() here as
   * HAL_GPIO_EXTI_IRQHandler() already does it
   */
  
  if(GPIO_Pin == BNRG_SPI_EXTI_PIN) {
    
    while (HAL_GPIO_ReadPin(BNRG_SPI_EXTI_PORT, BNRG_SPI_EXTI_PIN) == GPIO_PIN_SET) {
      
      uint8_t rx_buffer[255];
      uint8_t rx_bytes;
      
      /* Data are available from BlueNRG: read them through SPI */
      rx_bytes = BlueNRG_SPI_Read_All(&SpiHandle, rx_buffer, sizeof(rx_buffer)); 
      /* Check if there is data is so, send it to VCOM */
      if (rx_bytes > 0) {
        int i;
        for (i = 0; i < rx_bytes; i++) {
          PRINTF("0x%x, ", rx_buffer[i]);

          if(HAL_UART_Transmit(&UartHandle, (uint8_t*)&rx_buffer[i], 1, 300)!= HAL_OK) {
            Error_Handler();
          }
        }
        PRINTF("\n");
      }
      
      Clear_SPI_EXTI_Flag();
    }
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
