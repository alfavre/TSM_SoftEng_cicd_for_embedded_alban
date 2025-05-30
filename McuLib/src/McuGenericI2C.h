/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : McuGenericI2C.h
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : GenericI2C
**     Version     : Component 01.052, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2024-08-05, 17:56, # CodeGen: 845
**     Abstract    :
**         This component implements a generic I2C driver wrapper to work both with LDD and non-LDD I2C components.
**     Settings    :
**          Component name                                 : McuGenericI2C
**          Wait                                           : McuWait
**          Support STOP_NOSTART                           : no
**          Write Buffer Size                              : 32
**          non-LDD I2C                                    : Enabled
**            I2C                                          : McuGenericSWI2C
**          LDD I2C                                        : Disabled
**          RTOS                                           : Enabled
**            RTOS                                         : McuRTOS
**            Semaphore                                    : no
**          Init() on startup                              : yes
**     Contents    :
**         SelectSlave       - uint8_t McuGenericI2C_SelectSlave(uint8_t i2cAddr);
**         UnselectSlave     - uint8_t McuGenericI2C_UnselectSlave(void);
**         RequestBus        - void McuGenericI2C_RequestBus(void);
**         ReleaseBus        - void McuGenericI2C_ReleaseBus(void);
**         WriteBlock        - uint8_t McuGenericI2C_WriteBlock(void* data, uint16_t dataSize,...
**         ReadBlock         - uint8_t McuGenericI2C_ReadBlock(void* data, uint16_t dataSize,...
**         ReadBlockGeneric  - uint8_t McuGenericI2C_ReadBlockGeneric(void* data, uint16_t dataSize,...
**         ReadAddressWait   - uint8_t McuGenericI2C_ReadAddressWait(uint8_t i2cAddr, uint8_t *memAddr,...
**         ReadAddress       - uint8_t McuGenericI2C_ReadAddress(uint8_t i2cAddr, uint8_t *memAddr, uint8_t...
**         WriteAddress      - uint8_t McuGenericI2C_WriteAddress(uint8_t i2cAddr, uint8_t *memAddr, uint8_t...
**         ReadByte          - uint8_t McuGenericI2C_ReadByte(uint8_t i2cAddr, uint8_t *data);
**         WriteByte         - uint8_t McuGenericI2C_WriteByte(uint8_t i2cAddr, uint8_t data);
**         ReadByteAddress8  - uint8_t McuGenericI2C_ReadByteAddress8(uint8_t i2cAddr, uint8_t memAddr,...
**         WriteByteAddress8 - uint8_t McuGenericI2C_WriteByteAddress8(uint8_t i2cAddr, uint8_t memAddr,...
**         ReadWordAddress8  - uint8_t McuGenericI2C_ReadWordAddress8(uint8_t i2cAddr, uint8_t memAddr,...
**         WriteWordAddress8 - uint8_t McuGenericI2C_WriteWordAddress8(uint8_t i2cAddr, uint8_t memAddr,...
**         ProbeACK          - uint8_t McuGenericI2C_ProbeACK(void* data, uint16_t dataSize,...
**         GetSemaphore      - void* McuGenericI2C_GetSemaphore(void);
**         ScanDevice        - uint8_t McuGenericI2C_ScanDevice(uint8_t i2cAddr);
**         Deinit            - void McuGenericI2C_Deinit(void);
**         Init              - void McuGenericI2C_Init(void);
**
** * Copyright (c) 2013-2024, Erich Styger
**  * Web:         https://mcuoneclipse.com
**  * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**  * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**  * All rights reserved.
**  *
**  * Redistribution and use in source and binary forms, with or without modification,
**  * are permitted provided that the following conditions are met:
**  *
**  * - Redistributions of source code must retain the above copyright notice, this list
**  *   of conditions and the following disclaimer.
**  *
**  * - Redistributions in binary form must reproduce the above copyright notice, this
**  *   list of conditions and the following disclaimer in the documentation and/or
**  *   other materials provided with the distribution.
**  *
**  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**  * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file McuGenericI2C.h
** @version 01.00
** @brief
**         This component implements a generic I2C driver wrapper to work both with LDD and non-LDD I2C components.
*/         
/*!
**  @addtogroup McuGenericI2C_module McuGenericI2C module documentation
**  @{
*/         

#ifndef __McuGenericI2C_H
#define __McuGenericI2C_H

/* MODULE McuGenericI2C. */
#include "McuLib.h" /* SDK and API used */
#include "McuGenericI2Cconfig.h" /* configuration */


#ifdef __cplusplus
extern "C" {
#endif

#define McuGenericI2C_WRITE_BUFFER_SIZE   McuGenericI2C_CONFIG_WRITE_BUFFER_SIZE /* size of internal buffer used, set in the component properties */

typedef enum McuGenericI2C_EnumSendFlags_ {
  McuGenericI2C_SEND_STOP,        /* STOP is sent */
  McuGenericI2C_DO_NOT_SEND_STOP, /* STOP is not sent */
  McuGenericI2C_STOP_NOSTART      /* send STOP without START condition */
} McuGenericI2C_EnumSendFlags;

typedef enum McuGenericI2C_EnumStartFlags_ {
  McuGenericI2C_SEND_START,        /* Start is sent */
  McuGenericI2C_DO_NOT_SEND_START  /* Start is not sent */
} McuGenericI2C_EnumStartFlags;

typedef enum McuGenericI2C_EnumAckFlags_ {
  McuGenericI2C_SEND_LAST_ACK,   /* Nack after last received byte is sent */
  McuGenericI2C_DO_NOT_LAST_ACK  /* Nack after last received byte is not sent */
} McuGenericI2C_EnumAckFlags;

void McuGenericI2C_Init(void);
/*
** ===================================================================
**     Method      :  Init (component GenericI2C)
**
**     Description :
**         Initializes the driver.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuGenericI2C_Deinit(void);
/*
** ===================================================================
**     Method      :  Deinit (component GenericI2C)
**
**     Description :
**         Deinitializes the driver.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

uint8_t McuGenericI2C_ReadAddress(uint8_t i2cAddr, uint8_t *memAddr, uint8_t memAddrSize, uint8_t *data, uint16_t dataSize);
/*
** ===================================================================
**     Method      :  ReadAddress (component GenericI2C)
**
**     Description :
**         Read from the device. This writes (S+i2cAddr+0), (memAddr),
**         (Sr+i2cAddr+1), (data)...(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**       * memAddr         - Pointer to device memory address
**         memAddrSize     - number of address bytes
**       * data            - Pointer to read buffer
**         dataSize        - Size of read buffer
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_WriteAddress(uint8_t i2cAddr, uint8_t *memAddr, uint8_t memAddrSize, uint8_t *data, uint16_t dataSize);
/*
** ===================================================================
**     Method      :  WriteAddress (component GenericI2C)
**
**     Description :
**         Write to the device: (S+i2cAddr+0), (memAddr), (data)...
**         (data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C address of device
**       * memAddr         - Pointer to device memory address
**         memAddrSize     - number of address bytes
**       * data            - Pointer to data write buffer
**         dataSize        - Size of data buffer in bytes
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void* McuGenericI2C_GetSemaphore(void);
/*
** ===================================================================
**     Method      :  GetSemaphore (component GenericI2C)
**
**     Description :
**         Returns the currently allocated semaphore.
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_ReadBlock(void* data, uint16_t dataSize, McuGenericI2C_EnumSendFlags flags);
/*
** ===================================================================
**     Method      :  ReadBlock (component GenericI2C)
**
**     Description :
**         Read from the device a block.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Read buffer
**         dataSize        - Size of read buffer
**         flags           - flags for the transaction
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_WriteBlock(void* data, uint16_t dataSize, McuGenericI2C_EnumSendFlags flags);
/*
** ===================================================================
**     Method      :  WriteBlock (component GenericI2C)
**
**     Description :
**         Write a block to the device.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Data write buffer
**         dataSize        - 
**         flags           - flags for the transaction
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void McuGenericI2C_RequestBus(void);
/*
** ===================================================================
**     Method      :  RequestBus (component GenericI2C)
**
**     Description :
**         Starts a critical section for accessing the bus.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuGenericI2C_ReleaseBus(void);
/*
** ===================================================================
**     Method      :  ReleaseBus (component GenericI2C)
**
**     Description :
**         Finishes a critical section for accessing the bus.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

uint8_t McuGenericI2C_SelectSlave(uint8_t i2cAddr);
/*
** ===================================================================
**     Method      :  SelectSlave (component GenericI2C)
**
**     Description :
**         Selects the slave device on the bus. Method might use a
**         semaphore to protect bus access.
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_UnselectSlave(void);
/*
** ===================================================================
**     Method      :  UnselectSlave (component GenericI2C)
**
**     Description :
**         Unselects the device. Method will release a used a semaphore.
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_ReadByteAddress8(uint8_t i2cAddr, uint8_t memAddr, uint8_t *data);
/*
** ===================================================================
**     Method      :  ReadByteAddress8 (component GenericI2C)
**
**     Description :
**         Read a byte from the device using an 8bit memory address.
**         This writes (S+i2cAddr+0), (memAddr), (Sr+i2cAddr+1), (data)..
**         .(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**         memAddr         - Device memory address
**       * data            - Pointer to read buffer (single byte)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_WriteByteAddress8(uint8_t i2cAddr, uint8_t memAddr, uint8_t data);
/*
** ===================================================================
**     Method      :  WriteByteAddress8 (component GenericI2C)
**
**     Description :
**         Write a byte to the device using an 8bit memory address:
**         (S+i2cAddr+0), (memAddr), (data)...(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C address of device
**         memAddr         - Device memory address
**         data            - Data value
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_ScanDevice(uint8_t i2cAddr);
/*
** ===================================================================
**     Method      :  ScanDevice (component GenericI2C)
**
**     Description :
**         Checks if a device responds on the bus with an ACK.
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - 7bit I2C device address
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_ProbeACK(void* data, uint16_t dataSize, McuGenericI2C_EnumSendFlags flags, uint16_t WaitTimeUS);
/*
** ===================================================================
**     Method      :  ProbeACK (component GenericI2C)
**
**     Description :
**         Accesses the bus to check if the device responds with an ACK
**         (ACK polling).
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Data write buffer
**         dataSize        - 
**         flags           - flags for the transaction
**         WaitTimeUS      - Waiting time in microseconds
**                           to wait for the ACK on the bus.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_ReadBlockGeneric(void* data, uint16_t dataSize, McuGenericI2C_EnumSendFlags flags, McuGenericI2C_EnumStartFlags flagsStart, McuGenericI2C_EnumAckFlags flagsAck);
/*
** ===================================================================
**     Method      :  ReadBlockGeneric (component GenericI2C)
**
**     Description :
**         Read from the device a block with using additional control
**         and flags.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Read buffer
**         dataSize        - Size of read buffer
**         flagsSend       - flags for the send transaction
**         flagsStart      - Start flags
**         flagsAck        - Acknowledge flags
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_ReadWordAddress8(uint8_t i2cAddr, uint8_t memAddr, uint16_t *data);
/*
** ===================================================================
**     Method      :  ReadWordAddress8 (component GenericI2C)
**
**     Description :
**         Read a word from the device using an 8bit memory address.
**         This writes (S+i2cAddr+0), (memAddr), (Sr+i2cAddr+1), (data)..
**         .(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**         memAddr         - Device memory address
**       * data            - Pointer to read buffer (single byte)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_WriteWordAddress8(uint8_t i2cAddr, uint8_t memAddr, uint16_t data);
/*
** ===================================================================
**     Method      :  WriteWordAddress8 (component GenericI2C)
**
**     Description :
**         Write a word to the device using an 8bit memory address:
**         (S+i2cAddr+0), (memAddr), (data)...(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C address of device
**         memAddr         - Device memory address
**         data            - Data value
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_ReadByte(uint8_t i2cAddr, uint8_t *data);
/*
** ===================================================================
**     Method      :  ReadByte (component GenericI2C)
**
**     Description :
**         Read a byte from the device. This writes (S+i2cAddr+0),
**         (Sr+i2cAddr+1),(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**       * data            - Pointer to read buffer (single byte)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_WriteByte(uint8_t i2cAddr, uint8_t data);
/*
** ===================================================================
**     Method      :  WriteByte (component GenericI2C)
**
**     Description :
**         Write a byte to the device: (S+i2cAddr+0).(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C address of device
**         data            - Data value
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuGenericI2C_ReadAddressWait(uint8_t i2cAddr, uint8_t *memAddr, uint8_t memAddrSize, uint16_t waitMs, uint8_t *data, uint16_t dataSize);
/*
** ===================================================================
**     Method      :  ReadAddressWait (component GenericI2C)
**
**     Description :
**         Same as ReadAddress, but with an optional wait between the
**         address and read. Read from the device. This writes
**         (S+i2cAddr+0), (memAddr), (Sr+i2cAddr+1), (data)...(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**       * memAddr         - Pointer to device memory address
**         memAddrSize     - number of address bytes
**         waitMs          - Wait time between the address part
**                           and reading the data part. Wait time can be
**                           zero.
**       * data            - Pointer to read buffer
**         dataSize        - Size of read buffer
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

/* END McuGenericI2C. */

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif
/* ifndef __McuGenericI2C_H */
/*!
** @}
*/
