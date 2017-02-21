/**
******************************************************************************
* @file    main.c 
* @author  CL
* @version V1.0.0
* @date    04-July-2014
* @brief   BlueNRG Virtual Com Port main file.
*          This is the FW application for running the BlueNRG GUI on BlueNRG
*          development Kits.
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
#include "main.h"
#include "cube_hal.h"

#include "osal.h"
#include "debug.h"
#include "stm32_bluenrg_ble.h"

/** @addtogroup X-CUBE-BLE1_Applications
 *  @{
 */

/** @defgroup Virtual_COM_Port
 *  @{
 */

/** @defgroup MAIN 
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
 
/** @defgroup MAIN_Private_Defines 
 * @{
 */
/* Private defines -----------------------------------------------------------*/

/* UART timeout values */
#define BLE_UART_SHORT_TIMEOUT 30
#define BLE_UART_LONG_TIMEOUT 300

/* HCI Packet types */
#define HCI_COMMAND_PKT		0x01
#define HCI_ACLDATA_PKT		0x02
#define HCI_SCODATA_PKT		0x03
#define HCI_EVENT_PKT		  0x04
#define HCI_VENDOR_PKT		0xff

#define HCI_TYPE_OFFSET                 0
#define HCI_VENDOR_CMDCODE_OFFSET       1
#define HCI_VENDOR_LEN_OFFSET           2
#define HCI_VENDOR_PARAM_OFFSET         4

#define FW_VERSION_MAJOR    1
#define FW_VERSION_MINOR    9

/* Commands */
#define VERSION         0x01
#define EEPROM_READ     0x02
#define EEPROM_WRITE    0x03
#define BLUENRG_RESET   0x04
#define HW_BOOTLOADER   0x05

#define MAX_RESP_SIZE 255

#define RESP_VENDOR_CODE_OFFSET     1
#define RESP_LEN_OFFSET_LSB         2
#define RESP_LEN_OFFSET_MSB         3
#define RESP_CMDCODE_OFFSET         4
#define RESP_STATUS_OFFSET          5
#define RESP_PARAM_OFFSET           6

/* Types of vendor codes */
#define ERROR               0
/* Error codes */
#define UNKNOWN_COMMAND	    0x01
#define INVALID_PARAMETERS	0x12

#define RESPONSE            1
/* end of vendor codes */
/**
 * @}
 */

/** @defgroup MAIN_Private_Macros
 * @{
 */
/* Private macros ------------------------------------------------------------*/
#define PACK_2_BYTE_PARAMETER(ptr, param)  do{\
                *((uint8_t *)ptr) = (uint8_t)(param);   \
                *((uint8_t *)ptr+1) = (uint8_t)(param)>>8; \
                }while(0)
/**
 * @}
 */

/** @defgroup MAIN_Private_Variables
 * @{
 */ 
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Buffer used for transmission */
/* Buffer used for reception */
uint8_t uart_header[UARTHEADERSIZE];
uint8_t aRxBuffer[RXBUFFERSIZE];
/**
 * @}
 */

/** @defgroup MAIN_Private_Function_Prototypes
 * @{
 */ 
/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);
/**
 * @}
 */

 /** 
* @brief  Handle vendor command
* @param  cmd: the command
* @param  cmd_len: the length of the command
* @return None
*/
void handle_vendor_command(uint8_t* cmd, uint8_t cmd_len)
{
  int unsupported = 0;
  uint8_t len = 0;
  uint8_t response[MAX_RESP_SIZE];
  int i;
  
  response[0] = HCI_VENDOR_PKT;
  response[RESP_VENDOR_CODE_OFFSET] = RESPONSE;
  response[RESP_CMDCODE_OFFSET] = cmd[HCI_VENDOR_CMDCODE_OFFSET];
  response[RESP_STATUS_OFFSET] = 0;  
  
  if (cmd[HCI_TYPE_OFFSET] == HCI_VENDOR_PKT) {
    switch (cmd[HCI_VENDOR_CMDCODE_OFFSET]) {
    case VERSION:
      response[RESP_PARAM_OFFSET] = FW_VERSION_MAJOR;
      response[RESP_PARAM_OFFSET+1] = FW_VERSION_MINOR;
      len = 2;
      break;

    case BLUENRG_RESET:   
      BlueNRG_RST();
      break;

    case HW_BOOTLOADER:
      BlueNRG_HW_Bootloader();
      break;

    default:
      unsupported = 1;
    }
  } else {
    unsupported = 1;
  }
  
  if (unsupported) {
    response[RESP_STATUS_OFFSET] = UNKNOWN_COMMAND;
  }
  
  len += 2; // Status and Command code
  PACK_2_BYTE_PARAMETER(response + RESP_LEN_OFFSET_LSB, len);
  len += RESP_CMDCODE_OFFSET;
 
  PRINTF("From Nucleo to GUI: ");
  for (i = 0; i < len; i++) {
    PRINTF("0x%x, ", response[i]);
  }
  PRINTF("\n");  
  
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)response, len, BLE_UART_LONG_TIMEOUT) != HAL_OK) {
    Error_Handler();
  }
}

/** 
* @brief  Init the VCOM.
* @param  None
* @return None
*/
void vcom_init(void)
{
  /*## Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = ODD parity
      - BaudRate = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;
  
  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
}

static void wait(void)
{
  volatile uint32_t i;
  for (i=0; i<200000; i++) __NOP();
}

/* Private functions ---------------------------------------------------------*/
static void blink_led(void)
{
  BSP_LED_Toggle(LED2);
  wait();
  BSP_LED_Toggle(LED2); 
  wait();
}

static void error_stop(void)
{
  while (1) {
    blink_led();
  }
}

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();
  /* Enable USART1 clock */
  USARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
    
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
    
  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART1 */
  //HAL_NVIC_SetPriority(USARTx_IRQn, 4, 0);
  //HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{  
  /* STM32Cube HAL library initialization:
   *  - Configure the Flash prefetch, Flash preread and Buffer caches
   *  - Systick timer is configured by default as source of time base, but user 
   *    can eventually implement his proper time base source (a general purpose 
   *    timer for example or other time source), keeping in mind that Time base 
   *    duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
   *    handled in milliseconds basis.
   *  - Low Level Initialization
   */
  HAL_Init();
  
  /* Configure LED2 */
  BSP_LED_Init(LED2);
  
  /* Configure the User Button in GPIO Mode */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

  /* Configure the system clock */
  SystemClock_Config();
  
  /* Initialize the VCOM interface */
  vcom_init();
  
  /* Initialize the BlueNRG SPI driver */
  BNRG_SPI_Init();
  
  /* Reset BlueNRG hardware */
  BlueNRG_RST(); 
  
  while (1) {
    HAL_StatusTypeDef status;
    uint8_t len;
    int i;
    
    //FIXME: move to interrupt mode for vcom as blocking is flawed and will fail after timeout
        
    /* Read uart header */
    status = HAL_UART_Receive(&UartHandle, (uint8_t *)uart_header, UARTHEADERSIZE, BLE_UART_SHORT_TIMEOUT);
    if( status != HAL_OK && status != HAL_TIMEOUT)
    {
      Error_Handler();
    }

    if (status == HAL_OK) {
      // We actually received data from the GUI
      
      PRINTF("From GUI: ");
             
      for (i = 0; i < UARTHEADERSIZE; i++) {
        PRINTF("0x%x, ", uart_header[i]);
      }
       
      len = uart_header[UARTHEADERSIZE-1];
      
      if (len > 0) {
        /*## Put UART peripheral in reception process ###########################*/  
        /* Any data received will be stored "aRxBuffer" buffer */    
        if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, len, BLE_UART_LONG_TIMEOUT) != HAL_OK)
        {
          Error_Handler();
        }
      }
      
      for (i = 0; i < len; i++) {
        PRINTF("0x%x, ", aRxBuffer[i]);
      }
      PRINTF("\n");
      
      /* write data received from the vcom to the BlueNRG chip via SPI */
      if (uart_header[HCI_TYPE_OFFSET] == HCI_COMMAND_PKT) {
        // This is an HCI command so pass it to BlueNRG via SPI
        Hal_Write_Serial(uart_header, aRxBuffer, UARTHEADERSIZE, len);        
      } else {
        // Process the command locally without bothering with the BlueNRG chip
        handle_vendor_command(uart_header, UARTHEADERSIZE);
      }
    }

    while (HAL_GPIO_ReadPin(BNRG_SPI_EXTI_PORT, BNRG_SPI_EXTI_PIN) == GPIO_PIN_SET) {
      uint8_t rx_buffer[255];
      uint8_t rx_bytes;
      
      /* Data are available from BlueNRG: read them through SPI */
      rx_bytes = BlueNRG_SPI_Read_All(rx_buffer, sizeof(rx_buffer));
      /* Check if there is data is so, send it to VCOM */
      if (rx_bytes > 0) {
        int i;
        
        PRINTF("From BlueNRG to GUI: ");
              
        for (i = 0; i < rx_bytes; i++) {
          PRINTF("0x%x, ", rx_buffer[i]);

          if(HAL_UART_Transmit(&UartHandle, (uint8_t*)&rx_buffer[i], 1, BLE_UART_LONG_TIMEOUT)!= HAL_OK) {
            Error_Handler();
          }         
        }
        PRINTF("\n");
      }
    }
  }
}

/**
  * @brief  UART error callbacks
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @param  UART handle pointer  
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  blink_led();
  PRINTF("HAL_UART_ErrorCallback\n");
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  PRINTF("Error_Handler\n");
  /* Blink LED2 */
  while(1)
  {
    BSP_LED_Toggle(LED2);
    HAL_Delay(100);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
