/**
  @page BlueNRG Expansion Board for STM32 Nucleo Boards VCOM Port Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    readme.txt  
  * @author  CL/AST  
  * @version V0.0.1
  * @date    28-May-2014
  * @brief   This application is an example to be loaded in order to use the
  *          BlueNRG GUI with BlueNRG development platforms.
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

- Virtual_COM_Port is the application to be loaded in order to use the tools for
  updating the BlueNRG firmware on the X-NUCLEO-IDB04A1 and X-NUCLEO-IDB05A1 expansion boards
  (see readme.txt in folder Utilities/PC_Software/FlashUpaterTool).
- User can also use this project in order to port the BlueNRG VCOM application to his
  specific BlueNRG PCB (assuming that the customer PCB has a USB or RS232 I/O port
  available for PC connection).
- This application provides an interface compliant with the Bluetooth Low Energy DTM
  test commands.
- This application is not a reference application to be used for BlueNRG application
  development and evaluation. 
  
@par Hardware and Software environment

  - This example runs on STM32 Nucleo devices with BlueNRG STM32 expansion board
    (X-NUCLEO-IDB04A1 or X-NUCLEO-IDB05A1)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-L053R8 RevC board
    - NUCLEO-F401RE RevC board
    - NUCLEO-L476RG RevC board
    and can be easily tailored to any other supported device and development board.
    This example runs also on the NUCLEO-F411RE RevC board, even if the chip could
    be not exploited at its best since the projects are configured for the
    NUCLEO-F401RE target board.
  - This example is currently aligned with the STSW-BLUENRG-DK 1.9.0
    

@par How to use it ? 

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

 - IMPORTANT NOTE: To avoid issues with USB connection (mandatory if you have USB 3.0), it is   
   suggested to update the ST-Link/V2 firmware for STM32 Nucleo boards to the latest version.
   Please refer to the readme.txt file in the Applications directory for details.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
