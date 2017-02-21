/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
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
#include "cube_hal.h"

/* USER CODE BEGIN Includes */
#include <string.h>
#include "hal_types.h"
#include "hci.h"
#include "eddystone_beacon.h"

#include "stm32_bluenrg_ble.h"
#include "stm32xx_it.h"
#include "component.h"
#include "SensorTile_accelero.h"

/* USER CODE END Includes */

/** @addtogroup X-CUBE-BLE1_Applications
 *  @{
 */

/** @defgroup Beacon
 *  @{
 */

/** @defgroup MAIN 
 * @{
 */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/* USER CODE END PV */

static void *LSM303AGR_X_0_handle = NULL;
/** @defgroup MAIN_Private_Function_Prototypes
 * @{
 */
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void BlueNRG_Init(void);
void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/* USER CODE END PFP */
/**
 * @}
 */

/* USER CODE BEGIN 0 */
void EnterStopMode(void);
/* USER CODE END 0 */

/**
 * @brief  Main function to show how to use the BlueNRG Bluetooth Low Energy
 *         expansion board to implement a Eddystone Beacon device. 
 *
 * @param  None
 * @retval None
 */
int ii = 0;
char buf[21];
SensorAxes_t acceleration;

void initAcceleroSensor(void)
{
  if (BSP_ACCELERO_Init( LSM303AGR_X_0, &LSM303AGR_X_0_handle ) != COMPONENT_OK)
  {
    while(1);
  }  
  BSP_ACCELERO_Sensor_Enable( LSM303AGR_X_0_handle );
}

void Accelero_Sensor_Handler( void *handle )
{
  uint8_t id;
  uint8_t status;
  
  BSP_ACCELERO_Get_Instance( handle, &id );
  
  BSP_ACCELERO_IsInitialized( handle, &status );

  if ( BSP_ACCELERO_Get_Axes( handle, &acceleration ) == COMPONENT_ERROR )
  {
    acceleration.AXIS_X = 0;
    acceleration.AXIS_Y = 0;
    acceleration.AXIS_Z = 0;
  }
  else
  {
    
  }
}

int main(void)
{
  //HAL_EnableDBGStopMode();

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  initAcceleroSensor();
  /* Initialize the BlueNRG SPI */
  BNRG_SPI_Init();
  
  /* Initialize the BlueNRG HCI */
  HCI_Init();

  /* Reset BlueNRG hardware */
  BlueNRG_RST();

  /* Init BlueNRG protocol */
  BlueNRG_Init();

 //   sprintf(buf,"\r\nhello:%d!",i);
 //   UpdateAdvStr(buf);
  /* Initialize beacon services */
  if (EDDYSTONE_BEACON_TYPE & EDDYSTONE_UID_BEACON_TYPE)
  {
    EddystoneUID_Start();
  }
  if (EDDYSTONE_BEACON_TYPE & EDDYSTONE_URL_BEACON_TYPE)
  {
    EddystoneURL_Start();
  }

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

    HCI_Process();
//    EnterStopMode();
    
    HAL_Delay(10);
    Accelero_Sensor_Handler(LSM303AGR_X_0_handle);
//    sprintf(buf,"%d,%d,%d",acceleration.AXIS_X,acceleration.AXIS_Y,acceleration.AXIS_Z);
//    sprintf(buf,"x:%d,y:%d,z:%d",acceleration.AXIS_X,acceleration.AXIS_Y,acceleration.AXIS_Z);
    buf[0] = acceleration.AXIS_X >> 8;
    buf[1] = acceleration.AXIS_X & 0xff;    
    buf[2] = acceleration.AXIS_Y >> 8;
    buf[3] = acceleration.AXIS_Y & 0xff;
    buf[4] = acceleration.AXIS_Z >> 8;
    buf[5] = acceleration.AXIS_Z & 0xff;
    UpdateAdvStr(buf);
  }
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif


void EnterStopMode(void)
{
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}

/**
 * @brief  EXTI line detection callback.
 * @param  uint16_t GPIO_Pin Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  HCI_Isr();
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
