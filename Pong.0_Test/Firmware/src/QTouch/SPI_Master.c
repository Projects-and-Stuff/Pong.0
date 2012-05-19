/*
 * \file
 *
 * \brief
 *
 * Copyright (C) 2009 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
#include "touch.h"

#if defined(_DEBUG_INTERFACE_) && defined(QDEBUG_SPI)

/*============================ INCLUDES ======================================*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "SPI_Master.h"
#include "QDebugTransport.h"

/*============================ IMPLEMENTATION ================================*/

/*============================================================================
Name    :   SPI_Master_Init
------------------------------------------------------------------------------
Purpose :   Initialize SPI Interface
Input   :   n/a
Output  :   n/a
Notes   :	Called from QDebug_Init in QDebug.c
============================================================================*/
void SPI_Master_Init (void)
{
// Note: All DDRs are set in lowlevelinit.c
	// Slave resets are handled in lowlevelinit.c

    // MOSI, SCK and SS are outputs. Pullup on MISO
    SPI_DDR |= (1<<SS) | (1<<MOSI) | (1<<SCK) ;
    SPI_DDR &=~(1<<MISO);
    SPI_PORT &=~ ((1<<SS) | (1<<SCK) | (1<<MOSI)) ;
    SPI_PORT |= (1<<MISO);

    //Enable SPI master mode SCK=fosc/2
    SPCR0 = (1<<MSTR0) | (1<<SPE0)|(0<<SPR10)|(0<<SPR00);
    SPSR0 |=(1<<SPI2X0);

    //Enable SPI master mode SCK=fosc/4
    //SPCR = (1<<MSTR0) | (1<<SPE0)|(0<<SPR01)|(0<<SPR00);
    //SPSR &=~(1<<SPI2X0);

    //Enable SPI master mode SCK=fosc/8
    //SPCR = (1<<MSTR0) | (1<<SPE0)|(0<<SPR01)|(1<<SPR00);
    //SPSR |=(1<<SPI2X0);

    //Enable SPI master mode SCK=fosc/16
    //SPCR = (1<<MSTR0) | (1<<SPE0)|(1<<SPR00);
    //SPSR &=~(1<<SPI2X0);

    //Enable SPI master mode SCK=fosc/32
    //SPCR = (1<<MSTR) | (1<<SPE)|(1<<SPR1)|(0<<SPR0);
    //SPSR |=(1<<SPI2X);
}


/*============================================================================
Name    :   SPI_Send_Byte
------------------------------------------------------------------------------
Purpose :   Send and read one byte using SPI Interface
Input   :   Data to send to SPI Slave
Output  :   Data read from SPI Slave
Notes   :	Called from SPI_Send_Message
============================================================================*/
#pragma inline
uint8_t SPI_Send_Byte(uint8_t c)
{
 uint8_t temp;


    SPDR0 = c;   			            // Byte 1

	while (!(SPSR0 & (1<<SPIF0)));       // Wait until byte is sent

    temp=SPDR0;         					// Clear SPIF Flag



	DELAYUS(75);

    return temp;
}


/*============================================================================
Name    :   SPI_Send_Message
------------------------------------------------------------------------------
Purpose :   Send and read one frame using SPI Interface
Input   :   n/a
Output  :   n/a
Notes   :	Called from Send_Message in QDebugTransport.c
============================================================================*/
void SPI_Send_Message(void)
{
    uint16_t i;
	uint8_t FrameInProgress;

	// Send our message upstream
    for (i=0; i <= TX_index; i++)
    {
		FrameInProgress = RxHandler(SPI_Send_Byte(TX_Buffer[i]));
    }

	// Do we need to receive even more bytes?
	while (FrameInProgress)
		FrameInProgress = RxHandler(SPI_Send_Byte(0));

}

#endif/* _DEBUG_INTERFACE_*/
