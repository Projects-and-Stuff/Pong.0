/*******************************************************************************
*   $FILE:  QDebug.h
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

#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

/*----------------------------------------------------------------------------
nested include files
----------------------------------------------------------------------------*/
#include "touch_api.h"
#include "QDebugSettings.h"
/*======================== MANIFEST CONSTANTS ================================*/
#if (defined (QDEBUG_SPI) || defined (QDEBUG_SPI_BB))
   #define _OPT_SRAM_
#endif

/*! \name Subscription definitions.
 */
//! @{
#define SUBS_SIGN_ON     	         0
#define SUBS_GLOBAL_CONFIG	         1
#define SUBS_SENSOR_CONFIG	         2
#define SUBS_SIGNALS     	         3
#define SUBS_REF         	         4
#define SUBS_DELTA       	         5
#define SUBS_STATES      	         6
#define SUBS_QM_BURST_LENGTHS        7
#define SUBS_TIMESTAMPS              8
#define SUBS_USER_DATA               9
//! @}
/*! \name PC commands.
 */
//! @{
#define QT_CMD_DUMMY                   0x10
#define QT_CMD_SET_SUBS                0x11
#define QT_CMD_SET_GLOBAL_CONFIG       0x12
#define QT_CMD_SET_CH_CONFIG           0x13
#define QT_CMD_SET_QM_BURST_LENGTHS    0x14
#define QT_CMD_SET_USER_DATA           0x15
//! @}
/*! \name Touch MCU data packets.
 */
//! @{
#define QT_DUMMY                       0x20
#define QT_SIGN_ON                     0x21
#define QT_GLOBAL_CONFIG               0x22
#define QT_SENSOR_CONFIG               0x23
#define QT_SIGNALS                     0x24
#define QT_REFERENCES                  0x25
#define QT_DELTAS                      0x26
#define QT_STATES                      0x27
#define QT_QM_BURST_LENGTHS            0x28
#define QT_TIMESTAMPS                  0x29
#define QT_USER_DATA                   0x2A

#ifdef _OPT_SRAM_
#define TX_PKT_HEADER_LENGTH               (5u)
#define TX_SIGN_ON_PKT_LENGTH             (18u)
#define TX_GLOBAL_CONFIG_PKT_LENGTH       (17u)
#ifdef _ROTOR_SLIDER_
  #define TX_SENSOR_CONFIG_PKT_LENGTH        (TX_PKT_HEADER_LENGTH + 1u + (num_sensors * 4u))    
#else 
  #define TX_SENSOR_CONFIG_PKT_LENGTH        (TX_PKT_HEADER_LENGTH + 1u + (num_sensors * 3u))    
#endif
#define TX_SIGNALS_PKT_LENGTH             (TX_PKT_HEADER_LENGTH + (QT_NUM_CHANNELS * 2u))
#define TX_REFERENCE_PKT_LENGTH           (TX_PKT_HEADER_LENGTH + (QT_NUM_CHANNELS * 2u))
#define TX_DELTA_PKT_LENGTH               (TX_PKT_HEADER_LENGTH + (num_sensors * 2u))
#define TX_STATE_PKT_LENGTH               (TX_PKT_HEADER_LENGTH + 2u + QT_NUM_SENSOR_STATE_BYTES + QT_MAX_NUM_ROTORS_SLIDERS) 
#define TX_BURST_LEN_PKT_LENGTH           (TX_PKT_HEADER_LENGTH + QT_NUM_CHANNELS)
#define TX_TIME_STAMPS_PKT_LENGTH         (TX_PKT_HEADER_LENGTH + 22u) 
#endif
//! @}
/*======================== EXTERN VARIABLES ==================================*/
extern qt_touch_lib_config_data_t qt_config_data;
extern int16_t qt_get_sensor_delta( uint8_t );

/*============================ PROTOTYPES ====================================*/
/*! \name Public functions.
 */
//! @{
/*! \brief This API initializes QDebug interface, including the low level
 * hardware interface (SPI, TWI, USART etc).
 * \note Must be called before using any other QDebug API.
 */
void QDebug_Init (void);
/*! \brief Command handler for the data received from QTouch Studio
 * \note This function should be called in the main loop after
 * measure_sensors to process the data received from QTOuch Studio.
 */
void QDebug_ProcessCommands (void);
/*! \brief Send data to QTouch Studio based on the subscription.
 * \param  qt_lib_flags:Change flag from measure_sensors.
 */
void QDebug_SendData (uint16_t qt_lib_flags);
/*! \brief Set subscription values.
 * \note  This function can be used directly in main to set data subscription
 * if 1way SPI interface is used.
 */
void QDebug_SetSubscriptions (uint16_t once, uint16_t change,
			      uint16_t allways);

#if !(defined(__AVR32__) || defined(__ICCAVR32__) || defined(_TOUCH_ARM_))
  extern void set_timer_period(uint16_t qt_measurement_period_msec);
#endif

//! @}

/*! \name Private functions.
 */
//! @{
/*! \brief Extract the data packet from QTouch Studio and set global config.
 * \note  Should only be called from the command handler.
 */
void Set_Global_Config (void);
/*! \brief Extract the data packet from QTouch Studio and set channel config.
 * \note  Should only be called from the command handler.
 */
void Set_Channel_Config (void);
/*! \brief Set Data Subscription values.
 * \note  Should only be called from the command handler.
 */
void Set_Subscriptions (void);
/*! \brief Extract the data packet from QTouch Studio and set measurement period.
 * \note  Should only be called from the command handler.
 */
void Set_Measurement_Period (void);
/*! \brief Extracts user data from QTouch Studio to touch mcu memory.
 * \param pdata: data pointer.
 * \note  The data can be binary data.
 */
void Set_QT_User_Data (uint8_t *);
#ifdef _QMATRIX_
/*! \brief Extract the data packet from QTouch Studio and set QMatrix burst lengths.
 * \note  Should only be called from the command handler.
 */
void Set_QM_Burst_Lengths (void);
#endif
/*! \brief Transmits a dummy packet if no other subscriptions are set.
 */
void Transmit_Dummy (void);
/*! \brief Transmits the sign on packet to QTouch Studio.
 */
void Transmit_Sign_On (void);
/*! \brief Transmits the global config struct to QTouch Studio.
 */
void Transmit_Global_Config (void);
/*! \brief Transmits the channel config struct to QTouch Studio.
 */
void Transmit_Sensor_Config (void);
/*! \brief Transmits the measurement values for each channel to QTouch Studio.
 */
void Transmit_Signals (void);
/*! \brief Transmits the channel reference values to QTouch Studio.
 */
void Transmit_Ref (void);
/*! \brief Transmits the channel delta values to QTouch Studio.
 */
void Transmit_Delta (void);
/*! \brief Transmits the state values to QTouch Studio.
 */
void Transmit_State (void);
/*! \brief Transmits user data to QTouch Studio.
 * \param pdata: data pointer.
 * \param c: length of data in bytes.
 * \note The data will be binary data.
 */
#ifdef _QMATRIX_
/*! \brief Transmits the QMatrix burst length values to QTouch Studio
 * \note This value is available for each channel
 */
void Transmit_Burst_Lengths(void);
#endif

void Transmit_QT_User_Data (uint8_t * pdata, uint16_t c);
/*! \brief Transmits the application execution timestamp values to QTouch Studio.
 * \note This value is a combination of current_time_ms_touch (high word) &
 * timer counter register (low word).
 */
void Transmit_Time_Stamps(uint16_t qt_lib_flags);
//! @}
#endif
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-EOF-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
