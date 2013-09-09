/*******************************************************************************
*   $FILE:  QDebugTransport.h
*   Atmel Corporation:  http://www.atmel.com \n
*   Support email:  touch@atmel.com
******************************************************************************/

/*  License
*   Copyright (c) 2010, Atmel Corporation All rights reserved.
*
*   Redistribution and use in source and binary forms, with or without
*   modification, are permitted provided that the following conditions are met:
*
*   1. Redistributions of source code must retain the above copyright notice,
*   this list of conditions and the following disclaimer.
*
*   2. Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
*   3. The name of ATMEL may not be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
*   THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
*   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
*   SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
*   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
*   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef DEBUGTRANSPORT_H_INCLUDED
#define DEBUGTRANSPORT_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif
/*============================ INCLUDES ======================================*/
#include "touch_api.h"
#include "QDebug.h"


/*============================ MACROS ========================================*/

#define MESSAGE_START	0x1B
#define INT(a, b)	((a << 8) | b)
#define STATE_IDLE      0
#define STATE_LENGTH1	1
#define STATE_LENGTH2	2
#define STATE_DATA      3

#ifdef _ROTOR_SLIDER_
#ifdef _OPT_SRAM_
   #define TX_BUFFER_SIZE 1
#else
   #define TX_BUFFER_SIZE (QT_NUM_CHANNELS*4)+10
#endif
   #define RX_BUFFER_SIZE (QT_NUM_CHANNELS*4)+10
#else
#ifdef _OPT_SRAM_
   #define TX_BUFFER_SIZE 1
#else
   #define TX_BUFFER_SIZE (QT_NUM_CHANNELS*3)+10
#endif
   #define RX_BUFFER_SIZE (QT_NUM_CHANNELS*3)+10
#endif

/*============================ EXTERNAL VARIABLES ==============================*/

extern uint8_t TX_Buffer[TX_BUFFER_SIZE];
extern uint16_t TX_index;
extern uint8_t RX_Buffer[RX_BUFFER_SIZE];
extern uint16_t RX_index;
extern uint8_t packet_crc;
extern uint8_t SequenceL;
extern uint8_t SequenceH;
extern unsigned int gMsTimeout;

/*============================ PROTOTYPES ====================================*/

/*! \brief Initialize the send and receive buffers.
 * \note Called from QDebug_Init.
 */
  void Init_Buffers (void);
void Set_datamode(unsigned char mode);

/*! \brief Puts one byte in the Transmit Buffer.
 * \param data: byte to be sent.
 * \note Called from QDebug_Init.
 */
  void PutChar (uint8_t data);

/*! \brief Puts two bytes in the Transmit Buffer.
 * \param data: 16bit data to be sent.
 * \note Big Endian. TX_index is post incremented.
 */
  void PutInt (uint16_t data);

/*! \brief Get one byte from the Receive Buffer.
 * \return uint8_t: byte received.
 * \note RX_index is post incremented.
 */
  uint8_t GetChar (void);

/*! \brief Send the content of the TX_Buffer to the USB Bridge using the
 * interface selected in QDebugSettings.h
 * \note Called from the transmit functions in QDebug.c.
 */
  void Send_Message (void);

/*! \brief Executes a master read transmission if TWI is selected as interface.
 * Checks if RX_Buffer has a valid frame
 * \return uint8_t: returns a true or false dependent on whether a valid frame is
 * available or not
 * \note Called from QDebug_ProcessCommands in QDebug.c.
 */
  uint8_t Receive_Message (void);

/*! \brief Handles the incoming bytes from the interface selected in
 * QDebugSettings.h and puts the bytes in the RX_Buffer data read by
 * the selected interface
 * \return uint8_t: returns a true if more data must be read,
 * returns a false if the frame is complete
 * \note Used by SPI and TWI receive handlers.
 */
  uint8_t RxHandler (uint8_t c);

#ifdef __cplusplus
}
#endif

#endif
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-EOF-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
