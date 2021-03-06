/* This source file is part of the ATMEL QTouch Library Release 5.0.0 */
/* This file is prepared for Doxygen automatic documentation generation. */
/*! \file *********************************************************************
 *
 * \brief  This file contains the QDebug public API that can be used to
 * transfer data from a Touch Device to QTouch Studio using the QT600
 * USB Bridge.
 *
 * - Compiler:           IAR EWAVR8 and GNU GCC for AVR8
 * - Supported devices:  ATTiny, ATMega & ATXMega.
 * - Userguide:          QTouch Library User Guide - doc8207.pdf.
 * - Support email:      touch@atmel.com
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/* Copyright (c) 2010 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */
/*============================ INCLUDES ======================================*/
#include "touch_api.h"

//! compile file only when QDebug is enabled.
#ifdef _DEBUG_INTERFACE_
#include "QDebug.h"
#include "QDebugTransport.h"
//#include "QDebugSettings.h"

#if !(defined(__AVR32__) || defined(__ICCAVR32__) || defined(_TOUCH_ARM_))
   #include "eeprom_access.h"
#endif

#if defined(QDEBUG_SPI)
   #include "SPI_Master.h"
#elif defined(QDEBUG_TWI)
   #include "TWI_Master.h"
#elif defined(QDEBUG_SPI_BB)
   #include "BitBangSPI_Master.h"
#else
   #warning "No Debug Interface is selected in QDebugSettings.h"
#endif

/*============================ GLOBAL VARIABLES ==============================*/
//! These must somehow be updated from the library.
uint8_t qgRefschanged;
uint8_t qgStateschanged;
qt_lib_siginfo_t lib_siginfo;

/*============================ EXTERN VARIABLES ==============================*/
extern uint8_t RX_Buffer[];
extern uint8_t num_sensors;
extern sensor_t sensors[];
#if defined (_QDEBUG_TIME_STAMPS_)
  extern uint16_t timestamp0_hword;
  extern uint16_t timestamp0_lword;
  extern uint16_t timestamp1_hword;
  extern uint16_t timestamp1_lword;
  extern uint16_t timestamp2_hword;
  extern uint16_t timestamp2_lword;
  extern uint16_t timestamp3_hword;
  extern uint16_t timestamp3_lword;
  extern uint16_t timestamp4_hword;
  extern uint16_t timestamp4_lword;
#endif
/*============================ STATIC VARIABLES ===============================*/
//! Subscriptions.

static uint16_t  delivery = 0;
static uint16_t  qgSubsOnce = 0;
static uint16_t  qgSubsChange = 0;
static uint16_t  qgSubsAllways = 0;
static uint16_t  qgLibraryChanges = 0;

/*=========================== FUNCTION PROTOTYPES =============================*/
#ifdef _OPT_SRAM_
static void Msg_Start_Routine(uint16_t msg_length);
static void Msg_End_Routine(void);
#endif
/*============================ IMPLEMENTATION ================================*/
/*! \brief This API initializes QDebug interface
 * 
 * \note Must be called before the main loop
 */
void
QDebug_Init (void)
{
   #if !(defined(__AVR32__) || defined(__ICCAVR32__) || defined(_TOUCH_ARM_))
      uint16_t eeprom_lib_version;
   #endif
   #if defined(QDEBUG_SPI)
      SPI_Master_Init();
   #elif defined(QDEBUG_TWI)
      TWI_Master_Init();
   #elif defined(QDEBUG_SPI_BB)
      BitBangSPI_Master_Init();
   #endif	
      qt_get_library_sig(&lib_siginfo);
      Init_Buffers();
      /* Eeprom settings sanity check */
   #if !(defined(__AVR32__) || defined(__ICCAVR32__) || defined(_TOUCH_ARM_))
      eeprom_lib_version = read_info_from_eeprom();
      if(eeprom_lib_version == lib_siginfo.library_version)
      {
         read_settings_from_eeprom();
      }
      else
      {
         write_info_to_eeprom(lib_siginfo.library_version);
         write_global_settings_to_eeprom();
         write_sensor_settings_to_eeprom();
         #ifdef _QMATRIX_
            write_burst_lenghts_to_eeprom();
         #endif
      }
   #endif
}

/*! \brief Command handler for the data received from QTouch Studio
 * \note This function should be called in the main loop after
 * measure_sensors to process the data received from QTOuch Studio.
 */
void
QDebug_ProcessCommands (void)
{
   uint8_t CommandID;

   /* Fill in the address for user data in memory */
   uint8_t *pdata = 0;

   if (Receive_Message() == 0)
      return;
   // Handle the commands
   CommandID = GetChar();
   switch (CommandID)
   {
   case QT_CMD_DUMMY:
      break;
   case QT_CMD_SET_SUBS:
      Set_Subscriptions();
      break;
   case QT_CMD_SET_GLOBAL_CONFIG:
      Set_Global_Config();
      break;
   case QT_CMD_SET_CH_CONFIG:
      Set_Channel_Config();
      break;
   #ifdef _QMATRIX_
      case QT_CMD_SET_QM_BURST_LENGTHS:
         Set_QM_Burst_Lengths();
         break;
   #endif
   case QT_CMD_SET_USER_DATA:
      Set_QT_User_Data(pdata);
      break;
   }
   RX_Buffer[0]=0;
   RX_index=0;	
}

/*! \brief Send data to QTouch Studio based on the subscription.
 * \param  qt_lib_flags:Change flag from measure_sensors.
 * \note This function should be called in the main loop after
 *  measure_sensors to send the measured touch data
 */
void
QDebug_SendData (uint16_t qt_lib_flags)
{
#if defined (_QDEBUG_TIME_STAMPS_)  
  uint8_t *pdata = NULL;
  uint16_t c = 0;
#endif  
   SequenceH = (SequenceH+1)&0x0F;
   /* Test if measure_sensors has reported change in key states or rotor/slider positions */
   if((qt_lib_flags & QTLIB_STATUS_CHANGE) ||
      (qt_lib_flags & QTLIB_ROTOR_SLIDER_POS_CHANGE) )
   {
      qgLibraryChanges |= (1<<SUBS_STATES);
   }
   /* Test if measure_sensors has reported change in at least one channel reference */
   if(qt_lib_flags & QTLIB_CHANNEL_REF_CHANGE)
   {
      qgLibraryChanges |= (1<<SUBS_REF);
   }	
#if defined (_QDEBUG_TIME_STAMPS_)
   qgSubsAllways |= (1<<SUBS_TIMESTAMPS);
   qgSubsAllways |= (1<<SUBS_USER_DATA);
#endif
   delivery = qgSubsAllways | qgSubsOnce | (qgLibraryChanges & qgSubsChange);

   if (delivery & (1<<SUBS_SIGN_ON))
      Transmit_Sign_On();
   if (delivery & (1<<SUBS_GLOBAL_CONFIG))
      Transmit_Global_Config();
   if (delivery & (1<<SUBS_SENSOR_CONFIG))
      Transmit_Sensor_Config();
   if (delivery & (1<<SUBS_SIGNALS))
      Transmit_Signals();
   if (delivery & (1<<SUBS_REF))
      Transmit_Ref();
   if (delivery & (1<<SUBS_STATES))
      Transmit_State();
   if (delivery & (1<<SUBS_DELTA))
      Transmit_Delta();
#ifdef _QMATRIX_
   if (delivery & (1<<SUBS_QM_BURST_LENGTHS))
      Transmit_Burst_Lengths();
#endif
#if defined (_QDEBUG_TIME_STAMPS_)
   if (delivery & (1<<SUBS_TIMESTAMPS))
      Transmit_Time_Stamps(qt_lib_flags);
   if (delivery & (1<<SUBS_USER_DATA))
      Transmit_QT_User_Data(pdata, c);
#endif   
   if (delivery == 0)
      Transmit_Dummy();
   // Reset masks until PC sends another request
   qgSubsOnce = 0;
   // Reset the changes we have just sent
   qgLibraryChanges &= ~delivery;	
}

/*! \brief Set subscription values.
 * \note  This function can be used directly in main to set data subscription
 * if 1way SPI interface is used.
 */
void
QDebug_SetSubscriptions (uint16_t once, uint16_t change, uint16_t allways)
{
   qgSubsOnce = once;
   qgSubsChange = change;
   qgSubsAllways = allways;
}

/*! \brief Set Data Subscription values.
 * \note  Should only be called from the command handler.
 */
void
Set_Subscriptions (void)
{
   uint16_t temp;

   temp = GetChar() << 8;			// Bit 8-15 
   uint16_t a = temp | GetChar();
   
   temp = GetChar() << 8;			// Bit 8-15
   uint16_t b = temp | GetChar();

   temp = GetChar() << 8;			// Bit 8-15
   uint16_t c = temp | GetChar();
   QDebug_SetSubscriptions(a, b, c);
}

/*! \brief Extract the data packet from QTouch Studio and set global config.
 * \note  Should only be called from the command handler.
 */
void
Set_Global_Config (void)
{
   qt_config_data.qt_recal_threshold   = (recal_threshold_t)GetChar();
   qt_config_data.qt_di                = GetChar();
   qt_config_data.qt_drift_hold_time   = GetChar();
   qt_config_data.qt_max_on_duration   = GetChar();
   qt_config_data.qt_neg_drift_rate    = GetChar();
   qt_config_data.qt_pos_drift_rate    = GetChar();
   qt_config_data.qt_pos_recal_delay   = GetChar();
   qt_measurement_period_msec          = (GetChar() << 8u);
   qt_measurement_period_msec          |= GetChar();
#if !(defined(__AVR32__) || defined(__ICCAVR32__) || defined(_TOUCH_ARM_))
   set_timer_period(qt_measurement_period_msec);
#endif
   #if !(defined(__AVR32__) || defined(__ICCAVR32__) || defined(_TOUCH_ARM_))
      write_global_settings_to_eeprom();
   #endif
}

/*! \brief Extract the data packet from QTouch Studio and set channel config.
 * \note  Should only be called from the command handler.
 */
void
Set_Channel_Config (void)
{
   uint8_t sensor_number;
   uint8_t sensor_aks_hyst;

   sensor_number = GetChar();
   sensors[sensor_number].threshold = GetChar();
   sensor_aks_hyst = GetChar();

   /* Set only the AKS and Hysteresis fields (00111011) */
  sensors[sensor_number].type_aks_pos_hyst =
    (sensors[sensor_number].
     type_aks_pos_hyst & (0xC4u)) | (sensor_aks_hyst & ~(0xC4u));

   #if !(defined(__AVR32__) || defined(__ICCAVR32__) || defined(_TOUCH_ARM_))
      write_sensor_settings_to_eeprom();
   #endif
}

#ifdef _QMATRIX_
/*! \brief Set QMatrix burst lengths
 * \note  Should only be called from the command handler.
 */
void
Set_QM_Burst_Lengths (void)
{
   uint8_t c = 0;
   while( c < QT_NUM_CHANNELS )
   {
      qt_burst_lengths[c++] = GetChar();
   }
   #if !(defined(__AVR32__) || defined(__ICCAVR32__) || defined(_TOUCH_ARM_))
      write_burst_lenghts_to_eeprom();
   #endif
}
#endif
/*! \brief Extracts user data from QTouch Studio to touch mcu memory
 * \param  data pointer
 * \note  The data can be binary data
 */
void
Set_QT_User_Data (uint8_t * pdata)
{
   uint16_t c = RX_Buffer[1];
#ifdef _OPT_SRAM_
   Msg_Start_Routine(4u + c);
#endif   
   while( c > 0 )
   {
      PutChar(*pdata++);
      c--;
   }
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif
}

/*! \brief Transmits a dummy packet if no other subscriptions are set
 * \note 
 */
void
Transmit_Dummy (void)
{
#ifdef _OPT_SRAM_
   Msg_Start_Routine(TX_PKT_HEADER_LENGTH);  
#endif   
   PutChar(QT_DUMMY);
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif  
}

/*! \brief Transmits the sign on packet to QTouch Studio
 * \note 
 */
void
Transmit_Sign_On (void)
{
#ifdef _OPT_SRAM_
  Msg_Start_Routine(TX_SIGN_ON_PKT_LENGTH);
#endif	
   PutChar(QT_SIGN_ON);
   PutInt(PROJECT_ID);
   PutChar(INTERFACE);
   PutChar(1);                         //PROTOCOL_TYPE
   PutChar(2);                         //PROTOCOL_VERSION
   PutChar(lib_siginfo.lib_sig_lword);	//LIB_TYPE
   PutInt(lib_siginfo.library_version);//LIB_VERSION
   PutInt(0);                          //LIB_VARIANT
   PutChar(QT_NUM_CHANNELS);
   PutInt(delivery);    // subscription info
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif   
}

/*! \brief Transmits the global config struct to QTouch Studio
 * \note 
 */
void
Transmit_Global_Config (void)
{
#ifdef _OPT_SRAM_
   Msg_Start_Routine(TX_GLOBAL_CONFIG_PKT_LENGTH);
#endif		
   PutChar(QT_GLOBAL_CONFIG);
   PutChar(qt_config_data.qt_recal_threshold);
   PutChar(qt_config_data.qt_di);
   PutChar(qt_config_data.qt_drift_hold_time);
   PutChar(qt_config_data.qt_max_on_duration);
   PutChar(qt_config_data.qt_neg_drift_rate);
   PutChar(qt_config_data.qt_pos_drift_rate);
   PutChar(qt_config_data.qt_pos_recal_delay);
   PutInt(qt_measurement_period_msec);
   PutInt(TICKS_PER_MS);
   PutChar(0); // Time_Setting
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif 
}

/*! \brief Transmits the channel config struct to QTouch Studio
 * \note 
 */
void
Transmit_Sensor_Config (void)
{
  uint8_t c = 0;
#ifdef _OPT_SRAM_  
   #ifdef _ROTOR_SLIDER_
     Msg_Start_Routine(TX_SENSOR_CONFIG_PKT_LENGTH);    
   #else 
     Msg_Start_Routine(TX_SENSOR_CONFIG_PKT_LENGTH);    
   #endif
#endif   
   PutChar(QT_SENSOR_CONFIG);
   #ifdef _ROTOR_SLIDER_
      PutChar(1);		// 1 = KRS
      for(c=0; c<num_sensors; c++)
      {
         PutChar(sensors[c].threshold);
         PutChar(sensors[c].type_aks_pos_hyst);
         PutChar(sensors[c].from_channel);
         PutChar(sensors[c].to_channel);
      }
   #else
      PutChar(0u);		// 0 = K
      for(c=0; c<num_sensors; c++)
      {
         PutChar(sensors[c].threshold);
         PutChar(sensors[c].type_aks_pos_hyst);
         PutChar(sensors[c].from_channel);
      }
   #endif
#ifdef _OPT_SRAM_
  Msg_End_Routine();
#else
  Send_Message();
#endif 
}

/*! \brief Transmits the measurement values for each channel to QTouch Studio
 * \note 
 */
void
Transmit_Signals (void)
{
   uint8_t c;
#ifdef _OPT_SRAM_
   Msg_Start_Routine(TX_SIGNALS_PKT_LENGTH);
#endif   
   PutChar(QT_SIGNALS);
   for(c=0; c<QT_NUM_CHANNELS; c++)
   {
      PutChar(qt_measure_data.channel_signals[c] >> 8);
      PutChar(qt_measure_data.channel_signals[c] & 0xFF);
   }
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif 
}

/*! \brief Transmits the channel reference values to QTouch Studio
 * \note 
 */
void
Transmit_Ref (void)
{
   uint8_t c;
#ifdef _OPT_SRAM_
   Msg_Start_Routine(TX_REFERENCE_PKT_LENGTH);
#endif   
   PutChar(QT_REFERENCES);
   for(c=0; c<QT_NUM_CHANNELS; c++)
   {
      PutChar(qt_measure_data.channel_references[c] >> 8);
      PutChar(qt_measure_data.channel_references[c] & 0xFF);
   }
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif 
}

/*! \brief Transmits the channel delta values to QTouch Studio
 * \note The value is equal to signal-reference
 */
void
Transmit_Delta (void)
{
   uint8_t c;
   uint16_t delta;
#ifdef _OPT_SRAM_
   Msg_Start_Routine(TX_DELTA_PKT_LENGTH);
#endif    
   PutChar(QT_DELTAS);
   for (c=0; c < num_sensors; c++)
   {
      delta=qt_get_sensor_delta(c);
      PutChar(delta >> 8);
      PutChar(delta & 0xFF);
   }
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif
}

/*! \brief Transmits the state values to QTouch Studio
 * \note On/Off condition for each sensor
 */
void
Transmit_State(void)
{
   uint8_t c;
#ifdef _OPT_SRAM_     
   Msg_Start_Routine(TX_STATE_PKT_LENGTH); 
#endif   
  PutChar(QT_STATES);                                    
  PutChar(QT_NUM_CHANNELS);
  PutChar(QT_MAX_NUM_ROTORS_SLIDERS);		                
  for (c = 0; c < QT_NUM_SENSOR_STATE_BYTES; c++)
  {
    PutChar(qt_measure_data.qt_touch_status.sensor_states[c]);
  }
#ifdef _ROTOR_SLIDER_   
  for (c=0; c<QT_MAX_NUM_ROTORS_SLIDERS; c++)
  {
    PutChar(qt_measure_data.qt_touch_status.rotor_slider_values[c]);
  }
#endif    
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif
}

#ifdef _QMATRIX_
/*! \brief Transmits the QMatrix burst length values to QTouch Studio
 * \note This value is available for each channel
 */
void
Transmit_Burst_Lengths(void)
{
#ifdef _OPT_SRAM_     
   Msg_Start_Routine(TX_BURST_LEN_PKT_LENGTH);
#endif 
   PutChar(QT_QM_BURST_LENGTHS);
   uint8_t c = 0;
   while( c < QT_NUM_CHANNELS )
   {
      PutChar(qt_burst_lengths[c++]);
   }
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif  
}
#endif
#if defined (_QDEBUG_TIME_STAMPS_)
/*! \brief Transmits the application execution timestamp values to QTouch Studio
 * \note This value is a combination of current_time_ms_touch (high word) &
 *  timer counter register (low word)
 */
void
Transmit_Time_Stamps(uint16_t qt_lib_flags)
{
#ifdef _OPT_SRAM_     
   Msg_Start_Routine(TX_TIME_STAMPS_PKT_LENGTH);
#endif
   PutChar(QT_TIMESTAMPS);
   PutInt(timestamp0_hword);
   PutInt(timestamp0_lword);
   PutInt(timestamp1_hword);
   PutInt(timestamp1_lword);
   PutInt(timestamp2_hword);
   PutInt(timestamp2_lword);
   PutInt(timestamp3_hword);
   PutInt(timestamp3_lword);
   PutInt(timestamp4_hword);
   PutInt(timestamp4_lword);
   PutInt(qt_lib_flags);
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif
}

/*! \brief Transmits user data to QTouch Studio
 * \param data pointer, length of data in bytes
 * \note The data will be binary data
 */
void
Transmit_QT_User_Data (uint8_t * pdata, uint16_t c)
{
#ifdef _OPT_SRAM_     
   Msg_Start_Routine(TX_PKT_HEADER_LENGTH + c); 
#endif
   PutChar(QT_USER_DATA);
   while( c > 0 )
  {
      PutChar(*pdata++);
      c--;
  }
#ifdef _OPT_SRAM_
   Msg_End_Routine();
#else
   Send_Message();
#endif
}
#endif

#ifdef _OPT_SRAM_
/*! \brief Transmits packet starting details to QTouch Studio
 * \param message length, length of packet data in bytes
 * \note The data will be binary data
 */
static void 
Msg_Start_Routine(uint16_t msg_length)
{
  PutChar (0x1B);		
  packet_crc = 0; 
  PutChar ((uint8_t)msg_length >> 8);			   
  PutChar ((uint8_t)msg_length);			
  SequenceL= (SequenceL+1)& 0x0F;	 
  PutChar ((SequenceH<<4)+SequenceL);   
}
/*! \brief Transmits packet CRC to QTouch Studio
 * \param none
 * \note none
 */
static void
Msg_End_Routine(void)
{
  PutChar (packet_crc);
}
#endif
#endif
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-EOF-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
