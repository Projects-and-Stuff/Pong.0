/*******************************************************************************
*   $FILE:  main.c
*   Atmel Corporation:  http://www.atmel.com \n
*   Support email:  avr@atmel.com
******************************************************************************/

/*  License
*   Copyright (c) 2009-2010, Atmel Corporation All rights reserved.
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

/*----------------------------------------------------------------------------
                            compiler information
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                include files
----------------------------------------------------------------------------*/
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #define __delay_cycles(n)     __builtin_avr_delay_cycles(n)
    #define __enable_interrupt()  sei()
#include "touch_api.h"
#ifdef _DEBUG_INTERFACE_
/* Include files for QTouch Studio integration */
#include "QDebug.h"
#include "QDebugTransport.h"
#endif
/*----------------------------------------------------------------------------
                            manifest constants
----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
                            type definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                prototypes
----------------------------------------------------------------------------*/

/*  initialise host app, pins, watchdog, etc    */
static void init_system( void );
/*  configure timer ISR to fire regularly   */
static void init_timer_isr( void );
/*  Assign the parameters values to global configuration parameter structure    */
static void qt_set_parameters( void );
/*  Configure the sensors */
static void config_sensors(void);

#if defined(_ROTOR_SLIDER_)

/*  Configure the sensors with rotors/sliders with keys */
static void config_rotor_sliders(void);

    #if (QT_NUM_CHANNELS == 4u)
        /*  Configure the sensors for 4 channel Key Rotor/sliders   */
        static void config_4ch_krs(void);
    #endif
    #if (QT_NUM_CHANNELS == 8u)
        /*  Configure the sensors for 8 channel Key Rotor/sliders   */
        static void config_8ch_krs(void);
    #endif
    #if (QT_NUM_CHANNELS == 16u)
        /*  Configure the sensors for 16 channel Key Rotor/sliders  */
        static void config_16ch_krs(void);
    #endif
    #if (QT_NUM_CHANNELS == 32u)
        /*  Configure the sensors for 32 channel Key Rotor/sliders  */
        static void config_32ch_krs(void);
    #endif
    #if (QT_NUM_CHANNELS == 56u)
       /*  Configure the sensors for 64 channel Key Rotor/sliders  */
       static void config_56ch_krs(void);
    #endif
#else
    /*  Configure the sensors for  Keys configuration */
    static void config_keys(void);
#endif /* _ROTOR_SLIDER_ */



/*----------------------------------------------------------------------------
                            Structure Declarations
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                QMatrix macros
----------------------------------------------------------------------------*/

#define GET_SENSOR_STATE(SENSOR_NUMBER) qt_measure_data.qt_touch_status.sensor_states[(SENSOR_NUMBER/8)] & (1 << (SENSOR_NUMBER % 8))
#define GET_ROTOR_SLIDER_POSITION(ROTOR_SLIDER_NUMBER) qt_measure_data.qt_touch_status.rotor_slider_values[ROTOR_SLIDER_NUMBER]

#ifdef TICKS_PER_MS
#undef TICKS_PER_MS
#define TICKS_PER_MS                1000u
#endif

/*----------------------------------------------------------------------------
                                 Pong.0 macros
----------------------------------------------------------------------------*/

// Find the best way to define the sensor layout, and the associated LED display layout. It may be easiest to define a struct.


/*----------------------------------------------------------------------------
                                global variables
----------------------------------------------------------------------------*/
/* Timer period in msec. */
uint16_t qt_measurement_period_msec = 25u;
#ifdef _DEBUG_INTERFACE_
uint16_t timestamp1_hword;
uint16_t timestamp1_lword;
uint16_t timestamp2_hword;
uint16_t timestamp2_lword;
uint16_t timestamp3_hword;
uint16_t timestamp3_lword;
#endif
/*----------------------------------------------------------------------------
                                extern variables
----------------------------------------------------------------------------*/
/* This configuration data structure parameters if needs to be changed will be
   changed in the qt_set_parameters function */
extern qt_touch_lib_config_data_t qt_config_data;
/* measurement data */
extern qt_touch_lib_measure_data_t qt_measure_data;
qt_touch_lib_measure_data_t *pqt_measure_data = &qt_measure_data;
/* Get sensor delta values */
extern int16_t qt_get_sensor_delta( uint8_t sensor);

#ifdef _QMATRIX_
   extern y_line_info_t y_line_info[NUM_Y_LINES];
   extern x_line_info_t x_line_info[NUM_X_LINES];

   /* Fill out the X-Line masks on the X- Port selected.
   * The order of X - Line numbering follows from the way the
   * X-Lines are filled as below.
   * Here, X0,X1,X2,X3,X4,X5,X6,X7 are on port-pin specified.
   * 1 - if to specify if X line is on PORT_X_1,pin on the same selected port.
   * 2 - if to specify if X line is on PORT_X_2,pin on the same selected port.
   * 3 - if to specify if X line is on PORT_X_3,pin on the same selected port.
   *
   * Note: 1. The Number entries should be based on NUM_X_LINES
   *          4 entries when NUM_X_LINES =4 and
   *          8 entries when NUM_X_LINES=8
   */

   #if (NUM_X_LINES==4)
   x_line_info_t x_line_info[NUM_X_LINES]= {
      FILL_OUT_X_LINE_INFO(  1,0u ),
      FILL_OUT_X_LINE_INFO(  1,1u ),
      FILL_OUT_X_LINE_INFO(  1,2u ),
      FILL_OUT_X_LINE_INFO(  1,3u )
   };
   #else
   x_line_info_t x_line_info[NUM_X_LINES]= {
      FILL_OUT_X_LINE_INFO(  1,0u ),
      FILL_OUT_X_LINE_INFO(  1,1u ),
      FILL_OUT_X_LINE_INFO(  1,2u ),
      FILL_OUT_X_LINE_INFO(  1,3u ),
      FILL_OUT_X_LINE_INFO(  1,4u ),
      FILL_OUT_X_LINE_INFO(  1,5u ),
      FILL_OUT_X_LINE_INFO(  1,6u ),
      FILL_OUT_X_LINE_INFO(  1,7u )
   };
   #endif /*(NUM_X_LINES==4)*/

   /* Fill out the Y-Line masks on the Y- Line port selected
   * The order of Y - Line numbering follows from the way the
   * Y-Lines are filled as below
   * Here, Y0,Y1,Y2,Y3 on 0,1,2,3
   * Note: 1. The Number entries should be based on NUM_X_LINES
   *          2 entries when NUM_Y_LINES=2
   *          4 entries when NUM_Y_LINES=4
   *          8 entries when NUM_Y_LINES=8
   */
   #if (NUM_Y_LINES==1)
   y_line_info_t ya_line_info[NUM_Y_LINES]= {
      FILL_OUT_YA_LINE_INFO(  0u )
   };
   y_line_info_t yb_line_info[NUM_Y_LINES]= {
         FILL_OUT_YB_LINE_INFO(  0u )
   };
   #elif (NUM_Y_LINES==2)
   y_line_info_t ya_line_info[NUM_Y_LINES]= {
      FILL_OUT_YA_LINE_INFO(  0u ),
      FILL_OUT_YA_LINE_INFO(  1u )
   };
   y_line_info_t yb_line_info[NUM_Y_LINES]= {
         FILL_OUT_YB_LINE_INFO(  0u ),
         FILL_OUT_YB_LINE_INFO(  1u )
   };
   #elif (NUM_Y_LINES==4)
   y_line_info_t ya_line_info[NUM_Y_LINES]= {
      FILL_OUT_YA_LINE_INFO(  0u ),
      FILL_OUT_YA_LINE_INFO(  1u ),
      FILL_OUT_YA_LINE_INFO(  2u ),
      FILL_OUT_YA_LINE_INFO(  3u )
   };
   y_line_info_t yb_line_info[NUM_Y_LINES]= {
         FILL_OUT_YB_LINE_INFO(  0u ),
         FILL_OUT_YB_LINE_INFO(  1u ),
         FILL_OUT_YB_LINE_INFO(  2u ),
         FILL_OUT_YB_LINE_INFO(  3u )
   };
   #else
   y_line_info_t ya_line_info[NUM_Y_LINES]= {
      FILL_OUT_YA_LINE_INFO(  0u ),
      FILL_OUT_YA_LINE_INFO(  1u ),
      FILL_OUT_YA_LINE_INFO(  2u ),
      FILL_OUT_YA_LINE_INFO(  3u ),
      FILL_OUT_YA_LINE_INFO(  4u ),
      FILL_OUT_YA_LINE_INFO(  5u ),
      FILL_OUT_YA_LINE_INFO(  6u )

   };
   y_line_info_t yb_line_info[NUM_Y_LINES]= {
         FILL_OUT_YB_LINE_INFO(  0u ),
         FILL_OUT_YB_LINE_INFO(  1u ),
         FILL_OUT_YB_LINE_INFO(  2u ),
         FILL_OUT_YB_LINE_INFO(  3u ),
         FILL_OUT_YB_LINE_INFO(  4u ),
         FILL_OUT_YB_LINE_INFO(  5u ),
         FILL_OUT_YB_LINE_INFO(  6u )

   };
   #endif /*(NUM_Y_LINES==1)*/
#endif /*_QMATRIX_*/

/*----------------------------------------------------------------------------
                                static variables
----------------------------------------------------------------------------*/

/* flag set by timer ISR when it's time to measure touch */
static volatile uint8_t time_to_measure_touch = 0u;

/* current time, set by timer ISR */
static volatile uint16_t current_time_ms_touch = 0u;

/*============================================================================
Name    :   main
------------------------------------------------------------------------------
Purpose :   main code entry point
Input   :   n/a
Output  :   n/a
Notes   :
============================================================================*/

int main( void )
{
    /*status flags to indicate the re-burst for library*/
    uint16_t status_flag = 0u;
    uint16_t burst_flag = 0u;
    /* initialise host app, pins, watchdog, etc */
    init_system();

    /* Configure the Sensors as keys or Keys With Rotor/Sliders in this function */
    config_sensors();

    /* initialise touch sensing */
    qt_init_sensing();

    /*  Set the parameters like recalibration threshold, Max_On_Duration etc in this function by the user */
    qt_set_parameters( );


    /* configure timer ISR to fire regularly */
    init_timer_isr();


    /*  Address to pass address of user functions   */
    /*  This function is called after the library has made capacitive measurements,
    *   but before it has processed them. The user can use this hook to apply filter
    *   functions to the measured signal values.(Possibly to fix sensor layout faults)    */
    qt_filter_callback = 0;
#ifdef _DEBUG_INTERFACE_
		QDebug_Init();
#endif

    /* enable interrupts */
    __enable_interrupt();
#ifdef _DEBUG_INTERFACE_
  /* Process commands from PC */
    QDebug_ProcessCommands();
#endif

/* loop forever */
   for( ; ; )
   {
      if( time_to_measure_touch )
      {
         /*  clear flag: it's time to measure touch  */
         time_to_measure_touch = 0u;

         do {

            /*  one time measure touch sensors    */
            status_flag = qt_measure_sensors( current_time_ms_touch );
            burst_flag = status_flag & QTLIB_BURST_AGAIN;
#ifdef _DEBUG_INTERFACE_
		/* send debug data */
        QDebug_SendData(status_flag);
#endif
            /*Time critical host application code goes here*/

         }while (  burst_flag) ;
      }
#ifdef _DEBUG_INTERFACE_
  			/* Process commands from PC */
            QDebug_ProcessCommands();
#endif

      /*  Time Non-critical host application code goes here  */

   }
}

/*============================================================================
Name    :   qt_set_parameters
------------------------------------------------------------------------------
Purpose :   This will fill the default threshold values in the configuration
            data structure.But User can change the values of these parameters .
Input   :   n/a
Output  :   n/a
Notes   :   initialize configuration data for processing
============================================================================*/

static void qt_set_parameters( void )
{
    /*  This will be modified by the user to different values   */
    qt_config_data.qt_di              = DEF_QT_DI;
    qt_config_data.qt_neg_drift_rate  = DEF_QT_NEG_DRIFT_RATE;
    qt_config_data.qt_pos_drift_rate  = DEF_QT_POS_DRIFT_RATE;
    qt_config_data.qt_max_on_duration = DEF_QT_MAX_ON_DURATION;
    qt_config_data.qt_drift_hold_time = DEF_QT_DRIFT_HOLD_TIME;
    qt_config_data.qt_recal_threshold = DEF_QT_RECAL_THRESHOLD;
    qt_config_data.qt_pos_recal_delay = DEF_QT_POS_RECAL_DELAY;
}

/*============================================================================
Name    :   config_sensors
------------------------------------------------------------------------------
Purpose :   Configure the sensors
Input   :   n/a
Output  :   n/a
Notes   :   n/a
============================================================================*/
static void config_sensors(void)
{
#if defined(_ROTOR_SLIDER_)
    config_rotor_sliders();
#else /*    !_ROTOR_SLIDER_ OR ONLY KEYS    */
    config_keys();
#endif /*   _ROTOR_SLIDER_  */
}
/*============================================================================
Name    :   config_keys
------------------------------------------------------------------------------
Purpose :   Configure the sensors as keys only
Input   :   n/a
Output  :   n/a
Notes   :   n/a
============================================================================*/
#ifndef _ROTOR_SLIDER_
static void config_keys(void)
{
    /*  enable sensors 0..3: keys on channels 0..3  */
    qt_enable_key( CHANNEL_0, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_1, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_2, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_3, AKS_GROUP_1, 10u, HYST_6_25 );

#if(QT_NUM_CHANNELS >= 8u)
    /*  enable sensors 4..7: keys on channels 4..7   */
    qt_enable_key( CHANNEL_4, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_5, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_6, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_7, AKS_GROUP_1, 10u, HYST_6_25 );
#endif
#if (QT_NUM_CHANNELS >= 16u)
    /*  enable sensors 8..15: keys on channels 8..15    */
    qt_enable_key( CHANNEL_8, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_9, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_10, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_11, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_12, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_13, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_14, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_15, AKS_GROUP_1, 10u, HYST_6_25 );
#endif
#if (QT_NUM_CHANNELS >= 32u)
    /*  enable sensors 16..31: keys on channels 16..31  */
    qt_enable_key( CHANNEL_16, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_17, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_18, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_19, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_20, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_21, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_22, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_23, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_24, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_25, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_26, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_27, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_28, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_29, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_30, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_31, AKS_GROUP_1, 10u, HYST_6_25 );
#endif
#if (QT_NUM_CHANNELS >= 56u)
    qt_enable_key( CHANNEL_32, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_33, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_34, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_35, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_36, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_37, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_38, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_39, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_40, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_41, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_42, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_43, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_44, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_45, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_46, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_47, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_48, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_49, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_50, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_51, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_52, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_53, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_54, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_55, AKS_GROUP_1, 10u, HYST_6_25 );
#endif
}
#endif/*    _ROTOR_SLIDER_  */
/*============================================================================
  Name : config_rotor_sliders
------------------------------------------------------------------------------
Purpose: Configure the Sensors as keys and also as Rotors/Sliders
Input  : n/a
Output : n/a
Notes  : n/a
============================================================================*/
#if defined(_ROTOR_SLIDER_)
static void config_rotor_sliders(void)
{
#if (QT_NUM_CHANNELS == 4u)
    {
        /*  Call this function if library used is 4 channel library with KRS Configuration  */
        config_4ch_krs();
    }
#endif
#if (QT_NUM_CHANNELS == 8u)
    {
        /*  Call this function if library used is 8 channel library with KRS Configuration  */
        config_8ch_krs();
    }
#endif
#if (QT_NUM_CHANNELS == 16u)
    {
        /*  Call this function if library used is 16 channel library with KRS Configuration */
        config_16ch_krs();
    }
#endif
#if (QT_NUM_CHANNELS == 32u)
    {
        /*  Call this function if library used is 32 channel library with KRS Configuration */
        config_32ch_krs();
    }
#endif
#if (QT_NUM_CHANNELS == 56u)
{
    config_56ch_krs();
}
#endif
}

/*============================================================================
Name    :   config_4ch_krs
------------------------------------------------------------------------------
Purpose :   Configure the Sensors as keys and Rotor/Sliders for 4 channels only
Input   :   n/a
Output  :   n/a
Notes   :   n/a
============================================================================*/
#if (QT_NUM_CHANNELS == 4u)
static void config_4ch_krs(void)
{
    /*  enable sensor 0: a key on channel 0 */
    qt_enable_key( CHANNEL_0, AKS_GROUP_2, 10u, HYST_6_25 );

    /*  enable sensor 1: a slider on channels 0..2  */
    qt_enable_slider( CHANNEL_1, CHANNEL_3, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );


}
#endif /*QT_NUM_CHANNELS == 4u config_4ch_krs */

/*============================================================================
Name    :   config_8ch_krs
------------------------------------------------------------------------------
Purpose :   Configure the Sensors as keys and Rotor/Sliders for 8 channels only
Input   :   n/a
Output  :   n/a
Notes   :   n/a
============================================================================*/
#if (QT_NUM_CHANNELS == 8u)
static void config_8ch_krs(void)
{
	/*  enable sensor 0: a rotor on channels 0..3   */
    qt_enable_rotor( CHANNEL_0, CHANNEL_3, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );

    /*  enable sensors 1: wlider on channels 4..7 respectively  */
    qt_enable_slider( CHANNEL_4, CHANNEL_7, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );

}
#endif /*   QT_NUM_CHANNELS == 8u config_8ch_krs    */

/*============================================================================
Name    :   config_16ch_krs
------------------------------------------------------------------------------
Purpose :   Configure the Sensors as keys and Rotor/Sliders for 16 channels only
Input   :   n/a
Output  :   n/a
Notes   :   n/a
============================================================================*/
#if (QT_NUM_CHANNELS == 16u)
static void config_16ch_krs(void)
{

  /*  enable sensors 0..7: keys on channels 0..7  */
    qt_enable_key( CHANNEL_0, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_1, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_2, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_3, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_4, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_5, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_6, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_7, AKS_GROUP_1, 10u, HYST_6_25 );
    /*  enable sensor 8: a rotor on channels 8..11  */
    qt_enable_rotor( CHANNEL_8, CHANNEL_11, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );
    /*  enable sensors 9: sliders on channels 12..15 respectively   */
    qt_enable_slider( CHANNEL_12, CHANNEL_15, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );

}
#endif  /*  QT_NUM_CHANNELS == 16u config_16ch_krs  */

/*============================================================================
Name    :   config_32ch_krs
------------------------------------------------------------------------------
Purpose :   Configure the Sensors as keys and Rotor/Sliders for 32 channels only
Input   :   n/a
Output  :   n/a
Notes   :   n/a
============================================================================*/
#if (QT_NUM_CHANNELS == 32u)
static void config_32ch_krs(void)
{

   /*  enable sensors 0..7: keys on channels 0..7  */
    qt_enable_key( CHANNEL_0, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_1, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_2, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_3, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_4, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_5, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_6, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_7, AKS_GROUP_1, 10u, HYST_6_25 );
    /*  enable sensor 8: a rotor on channels 8..11   */
    qt_enable_rotor( CHANNEL_8, CHANNEL_11, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );
    /*  enable sensors 9: sliders on channels 12..15 respectively   */
    qt_enable_slider( CHANNEL_12, CHANNEL_15, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );
    /*  enable sensors 10..11: keys on channels 16..17  */
    qt_enable_key( CHANNEL_16, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_17, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_18, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_19, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_20, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_21, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_22, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_23, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_24, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_25, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_26, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_27, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_28, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_29, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_30, AKS_GROUP_1, 10u, HYST_6_25 );
   qt_enable_key( CHANNEL_31, AKS_GROUP_1, 10u, HYST_6_25 );
}
#endif/*    QT_NUM_CHANNELS == 32u config_32ch_krs  */

/*============================================================================
Name    :   config_56ch_krs
------------------------------------------------------------------------------
Purpose :   Configure the Sensors as keys and Rotor/Sliders for 56 channels only
Input   :   n/a
Output  :   n/a
Notes   :   n/a
============================================================================*/
#if (QT_NUM_CHANNELS == 56u)
static void config_56ch_krs(void)
{
    /*  enable sensors 0..7: keys on channels 0..7  */
    qt_enable_key( CHANNEL_0, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_1, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_2, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_3, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_4, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_5, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_6, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_7, AKS_GROUP_1, 10u, HYST_6_25 );
    /*  enable sensor 8: a rotor on channels 8..11  */
    qt_enable_rotor( CHANNEL_8, CHANNEL_11, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );
    /*  enable sensors 9: sliders on channels 12..15 respectively    */
    qt_enable_slider( CHANNEL_12, CHANNEL_15, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );
    /*  enable sensors 10..11: keys on channels 16..17  */
    qt_enable_key( CHANNEL_16, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_17, AKS_GROUP_1, 10u, HYST_6_25 );
    /*  enable sensors 12..28: keys on channels 18..34   */
    qt_enable_key( CHANNEL_18, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_19, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_20, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_21, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_22, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_23, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_24, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_25, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_26, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_27, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_28, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_29, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_30, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_31, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_32, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_33, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_34, AKS_GROUP_1, 10u, HYST_6_25 );
    /*  enable sensor 29: a rotor on channels 35..38  */
    qt_enable_rotor( CHANNEL_35, CHANNEL_38, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );
    /*  enable sensors 30: sliders on channels 39..42 respectively    */
    qt_enable_slider( CHANNEL_39, CHANNEL_42, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );
    /*  enable sensor 31: a rotor on channels 43..46  */
    qt_enable_rotor( CHANNEL_43, CHANNEL_46, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );
    /*  enable sensors 32: sliders on channels 47..50 respectively    */
    qt_enable_slider( CHANNEL_47, CHANNEL_50, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );
    /*  enable sensors 33..37: keys on channels 51..55  */
    qt_enable_key( CHANNEL_51, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_52, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_53, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_54, AKS_GROUP_1, 10u, HYST_6_25 );
    qt_enable_key( CHANNEL_55, AKS_GROUP_1, 10u, HYST_6_25 );

}
#endif /*QT_NUM_CHANNELS == 56u config_56ch_krs */
#endif /* _ROTOR_SLIDER_ */

/*============================================================================
Name    :   init_timer_isr
------------------------------------------------------------------------------
Purpose :   configure timer ISR to fire regularly
Input   :   n/a
Output  :   n/a
Notes   :
============================================================================*/

static void init_timer_isr( void )
{
    /*  Set timer period    */
    TCC0.PER = TICKS_PER_MS * qt_measurement_period_msec;
    /*  select clock source */
    TCC0.CTRLA = (TOUCH_DATA_T)4;
    /*  Set Comparre A interrupt to low level   */
    TCC0.INTCTRLB = 1u;
    /*  enable low lever interrupts in power manager interrupt control  */
    PMIC.CTRL |= 1u;
}

/*============================================================================
Name    :   set_timer_period
------------------------------------------------------------------------------
Purpose :   changed the timer period runtime
Input   :   qt_measurement_period_msec
Output  :   n/a
Notes   :
============================================================================*/
void set_timer_period(uint16_t qt_measurement_period_msec)
{
	/*  set timer compare value (how often timer ISR will fire,set to 1 ms interrupt) */
   TCC0.PER = TICKS_PER_MS * qt_measurement_period_msec;
}

/*============================================================================
Name    :   CCP write helper function written in assembly.
------------------------------------------------------------------------------
Purpose :   This function is written in assembly because of the timecritial
            operation of writing to the registers for xmega.
Input   :   address - A pointer to the address to write to.
            value   - The value to put in to the register.
Notes   :
============================================================================*/
void CCPWrite( volatile uint8_t * address, uint8_t value )
{
#ifdef __ICCAVR__
    asm("movw r30, r16");
#ifdef RAMPZ
    RAMPZ = 0;
#endif

    asm("ldi  r16,  0xD8 \n"
        "out  0x34, r16  \n"
        "st     Z,  r18  \n");

#elif defined __GNUC__
    volatile uint8_t * tmpAddr = address;
#ifdef RAMPZ
    RAMPZ = 0;
#endif
    asm volatile(
        "movw r30,  %0"	"\n\t"
        "ldi  r16,  %2"	"\n\t"
        "out   %3, r16"	"\n\t"
        "st     Z,  %1"
        :
        : "r" (tmpAddr), "r" (value), "M" (CCP_IOREG_gc), "m" (CCP)
        : "r16", "r30", "r31"
        );
#endif
}

/*============================================================================
Name    :   init_system
------------------------------------------------------------------------------
Purpose :   initialise host app, pins, watchdog, etc
Input   :   n/a
Output  :   n/a
Notes   :
============================================================================*/

static void init_system( void )
{
    uint8_t PSconfig;
    uint8_t clkCtrl;

    /*  Configure Oscillator and Clock source   */

    /*  Select Prescaler A divider as 4 and Prescaler B & C divider as (1,1) respectively.  */
    /*  Overall divide by 4 i.e. A*B*C  */
    PSconfig = (uint8_t) CLK_PSADIV_4_gc | CLK_PSBCDIV_1_1_gc;

    /*  Enable internal 32 MHz ring oscillator. */
    OSC.CTRL |= OSC_RC32MEN_bm;
    CCPWrite( &CLK.PSCTRL, PSconfig );

    /*  Wait until oscillator is ready. */
    while ( ( OSC.STATUS & OSC_RC32MRDY_bm ) == 0 );

    /*  Set the 32 MHz ring oscillator as the main clock source */
    clkCtrl = ( CLK.CTRL & ~CLK_SCLKSEL_gm ) | CLK_SCLKSEL_RC32M_gc;
    CCPWrite( &CLK.CTRL, clkCtrl );

    /*  Route clk signal to port pin    */
    /*  PORTCFG_CLKEVOUT = 0x03;    */
    /*  PORTE_DIRSET = 0x80;    */
}


/*============================================================================
Name    :   timer_isr
------------------------------------------------------------------------------
Purpose :   timer 1 compare ISR
Input   :   n/a
Output  :   n/a
Notes   :
============================================================================*/

ISR(TCC0_CCA_vect)
{
    /*  set flag: it's time to measure touch    */
    time_to_measure_touch = 1u;

    /*  update the current time  */
    current_time_ms_touch += qt_measurement_period_msec;
}

