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
volatile unsigned char probe_detect_timer = 0;
volatile unsigned short probe_detect_filter = 0;

//--- Externals from adc
volatile unsigned short adc_ch [ADC_CHANNEL_QUANTITY];


// Globals ---------------------------------------------------------------------
// volatile unsigned short timer_sync_xxx_ms = 0;
// volatile unsigned short timer_out4 = 0;

// parameters_typedef * pmem = (parameters_typedef *) (unsigned int *) FLASH_PAGE_FOR_BKP;	//en flash
// parameters_typedef mem_conf;


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);
unsigned char Probe_Detect_Ch1 (void);
void Probe_Detect_Update (void);
void Starts_Everything (void);
void Shutdown_Everything (void);
void Full_Working_Loop (void);


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
    Ena_Ch1_Off();
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
    Wait_ms(200);

    //-- Comms with ch1
    UsartChannel1Config ();
    Wait_ms(200);

    //-- Comms with Connectors Brd
    UsartConnConfig ();
    Wait_ms(200);

    //-- Comms with Encoders Brd
    UsartEncConfig ();
    Wait_ms(200);
    
    //-- TIM1 for signals module sequence ready
    TIM6_Init();    // for square times
    TIM7_Init();    // for sine times
    
    //-- Welcome Code
    UsartRpiSend("\r\nMicro Current - Main Board v2.0 - starting...\r\n");
    // Ena_Ch1_On();
    // Ena_Ch2_On();
    // Ena_Ch3_On();    // 3 and 4 only enable by connectors
    // Ena_Ch4_On();
    
    //-- Main Loop --------------------------
    while (1)
    {
        // update the rpi comms
        Comms_Update ();

	// update Connectors Brd comms
	Comms_Conn_Update ();

        // update the ch1 & probe comms
        Comms_Channel1_Update ();

        // update treatment state
        Treatment_Manager();

        // the update of led and buzzer on Treatment_Manager()
        // UpdateLed();
    }
}

//--- End of Main ---//


// Other Module Functions ------------------------------------------------------
void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (timer_standby)
        timer_standby--;

    if (timer_for_batt_report)
        timer_for_batt_report--;    

    if (probe_detect_timer)
        probe_detect_timer--;

    // i2c_driver_timeouts ();

    Comms_Timeouts ();
    
    Battery_Timeout ();
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

