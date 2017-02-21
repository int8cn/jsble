/**
  @page BlueNRG Expansion Board for STM32 Nucleo Boards Sample Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  CL/AST  
  * @version V0.0.1
  * @date    19-Feb-2015
  * @brief   Description of the BlueNRG SampleApp_DMA_LowPower application.
  ******************************************************************************
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
  @endverbatim

@par Example Description 

This sample application shows how to use BlueNRG Bluetooth Low Energy 
stack.
To test this application you need two STM32 Nucleo boards with their
respective BlueNRG STM32 expansion boards. One board needs to be configured
as Server-Peripheral role, while the other needs to be configured as Client-Central
role.
Before flashing the boards, please make sure to select the right configuration (SERVER
or CLIENT) for each board from your IDE;

During the connection and characteritics discovery phase the LED2 on the CLIENT is ON.
Once the previous procedure (which lasts about 40 seconds) has completed, the LED2 on
the CLIENT gets OFF.
The connection phase can be shortened by reducing the connection interval, but in this case
the power consumption will increase.
After connection has been established, by pressing the USER button on one board,
the micro on the other one gets toggled from RUN to STOP mode and viceversa.
Also, the USER button sets the STOP mode locally.

If you have only one STM32 Nucleo board, you can program it as SERVER and use as CLIENT
the BLE IOT app for Android devices available on the Play Store at
https://play.google.com/store/apps/details?id=com.stmicro.bleiot 
   
Current consumption could be monitored through an amperemeter connected to JP6.
This sample application uses the STM32 Cube Low Level low power optimizations
along with the DMA module for offloading the MCU.
The communication is done using a vendor specific profile.


@par Hardware and Software environment

  - This example runs on STM32 Nucleo devices with BlueNRG STM32 expansion board
    (X-NUCLEO-IDB04A1 or X-NUCLEO-IDB05A1)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-L053R8 RevC board
    - NUCLEO-F401RE RevC board
    - NUCLEO-L476RG RevC board
    and can be easily tailored to any 
    other supported device and development board.
    This example runs also on the NUCLEO-F411RE RevC board, even if the chip could
    be not exploited at its best since the projects are configured for the
    NUCLEO-F401RE target board.

@par How to use it? 

In order to make the program work, you must do the following:
 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - Open IAR toolchain (this firmware has been successfully tested with
   Embedded Workbench V7.50.3).
   Alternatively you can use the Keil uVision toolchain (this firmware
   has been successfully tested with V5.22).
   Alternatively you can use the System Workbench for STM32 (this firmware
   has been successfully tested with Version 1.12.0).
 - Rebuild all files and load your image into target memory.
 - Run the example.
 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
