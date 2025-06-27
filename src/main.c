//------------------------------------------------------------
// #### PROJECT MICRO-CURRENTS MAINBRD F103 - Custom Board ###
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### MAIN.C ###############################################
//------------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32f10x.h"
#include "hard.h"

#include "adc.h"
#include "dma.h"
#include "dac.h"
#include "tim.h"
#include "gpio.h"
#include "usart_channels.h"
#include "usart.h"

#include "comms.h"
#include "comms_conn.h"
#include "comms_channel1.h"
#include "comms_supply.h"
#include "treatment.h"
#include "test_functions.h"
#include "audio.h"
#include "battery.h"


#include <stdio.h>
#include <string.h>


// Private Types Constants and Macros ------------------------------------------
typedef enum {
    INIT,
    MAINS_SUPPLY,
    BATTERY_GOOD,
    BATTERY_LOW
    
    
} supply_states_e;

// Externals -------------------------------------------------------------------
//--- Externals from timers
volatile unsigned short timer_standby = 0;
volatile unsigned short timer_for_batt_report = 0;
volatile unsigned short wait_ms_var = 0;
volatile unsigned short timer_ch1_default = 0;

	    
//--- Externals from adc
volatile unsigned short adc_ch [ADC_CHANNEL_QUANTITY];


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);
void Set_Default_Config (void);
void Channel1_Send_Default (void);
void Channel1_Send_Default_Update (void);


// Module Functions ------------------------------------------------------------
int main (void)
{
    // Gpio Configuration.
    GpioInit();
    
    // Systick Timer Activation
    if (SysTick_Config(64000))
        SysTickError();

    // Hardware Tests
    // TF_Hardware_Tests ();

    // --- main program inits. ---

    // Disable all channels on startup
    // Ena_Ch1_Off();    // disable after UsartRpiConfig
    Ena_Ch2_Off();
    Ena_Ch3_Off();
    Ena_Ch4_Off();

    //-- ADC without DMA
    AdcConfig();

    //-- DAC init for signal generation
    DAC_Config ();
    DAC_Output1(0);
    DAC_Output2(0);

    //-- Audio things init
    TIM5_Init();    // for audio sine
    Audio_Init();
    
    //-- Comms with rasp
    Ena_Rpi_On ();
    UsartRpiConfig ();
    Ena_Ch1_Off();    // disable after UsartRpiConfig
    Wait_ms(200);
    Ena_Lcd_On ();
    
    //-- Comms with ch1
    UsartChannel1Config ();
    Wait_ms(200);

    //-- Comms with Connectors Brd
    UsartConnConfig ();
    Wait_ms(200);

    //-- Comms with Encoders Brd
    UsartEncConfig ();
    Wait_ms(200);

    //-- Comms with Supply Brd
    UsartSupplyConfig ();
    Wait_ms(200);
    
    //-- TIM1 for signals module sequence ready
    TIM6_Init();    // for square times
    TIM7_Init();    // for sine times
    
    //-- Welcome Code
    UsartRpiSend("\r\nMicro Current - Main Board v2.0 - starting...\r\n");

    //-- Set Default Config and send it to connectors
    Set_Default_Config ();

    unsigned char rpi_last_state = 2;
    //-- Main Loop --------------------------
    while (1)
    {
        // update the rpi comms & rpi state
        Comms_Update ();
	if (rpi_last_state != Comms_Rpi_Answering())
	{
	    UsartConnSend("\r\n");
	    rpi_last_state = Comms_Rpi_Answering ();
	    if (rpi_last_state)
		UsartConnSend("rpi is up\r\n");
	    else
		UsartConnSend("rpi is down\r\n");

	}

	// update Connectors Brd comms
	Comms_Conn_Update ();

        // update the ch1 & probe comms
        Comms_Channel1_Update ();

	// update supply comms
	Comms_Supply_Update ();
	    
        // update treatment state
        Treatment_Manager ();

        // the update of led and buzzer on Treatment_Manager()
        // UpdateLed();

	Channel1_Send_Default_Update ();
    }
}

//--- End of Main ---//


// Other Module Functions ------------------------------------------------------
void Set_Default_Config (void)
{

    Treatment_SetFrequency_Str (MODE_SQUARE, "20.00");
    Treatment_SetIntensity_Str (MODE_SQUARE, "100");
    Treatment_SetFrequency_Str (MODE_SINE, "20.00");
    Treatment_SetIntensity_Str (MODE_SINE, "100");
    Treatment_SetPolarity_Str ("alt");

    // send conf to connectors
    UsartConnSend("\r\n");
    UsartConnSend("polarity alt\r\n");

}


unsigned char ch1_send = 0;
void Channel1_Send_Default (void)
{
    timer_ch1_default = 500;
    ch1_send = 1;
}


void Channel1_Send_Default_Update (void)
{
    switch (ch1_send)
    {
    case 0:
	// do nothing
	break;

    case 1:
	// initial wait & freq send
	if (!timer_ch1_default)
	{
	    UsartChannel1Send("\r\n");
	    UsartChannel1Send("sine frequency 20.00\r\n");
	    UsartChannel1Send("square frequency 20.00\r\n");	    
	    timer_ch1_default = 100;
	    ch1_send++;
	}
	break;

    case 2:
	// wait & intensity send
	if (!timer_ch1_default)
	{
	    UsartChannel1Send("sine intensity 100\r\n");
	    UsartChannel1Send("square intensity 100\r\n");    
	    timer_ch1_default = 100;
	    ch1_send++;
	}
	break;

    case 3:
	// wait & polarity send
	if (!timer_ch1_default)
	{
	    UsartChannel1Send("polarity alt\r\n");
	    ch1_send = 0;
	}
	break;

    default:
	ch1_send = 0;
	break;
    }
}


void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (timer_standby)
        timer_standby--;

    if (timer_for_batt_report)
        timer_for_batt_report--;    

    Comms_Timeouts ();
    
    Battery_Timeout ();

    if (timer_ch1_default)
	timer_ch1_default--;
}


void SysTickError (void)
{
    //Capture systick error...
    while (1)
    {
        // if (LED)
        //     LED_OFF;
        // else
        //     LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}

//--- end of file ---//

