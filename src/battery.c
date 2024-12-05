//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### BATTERY.C ################################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "battery.h"
// #include "hard.h"
#include "adc.h"


#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    START_12V_EXT_CONVERTION,
    WAIT_12V_EXT_CONVERTION,
    START_BAT_A_CONVERTION,
    WAIT_BAT_A_CONVERTION,
    START_BAT_B_CONVERTION,
    WAIT_BAT_B_CONVERTION
    
} battery_states_e;

#define Sense_12V_Ext    ADC_Channel_4
#define Sense_Bat_A    ADC_Channel_5
#define Sense_Bat_B    ADC_Channel_6


// Externals -------------------------------------------------------------------



// Globals ---------------------------------------------------------------------
volatile unsigned short battery_timeout = 0;
unsigned short mains_input_voltage = 0;
unsigned short batt_a_voltage = 0;
unsigned short batt_b_voltage = 0;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Battery_Timeout (void)
{
    if (battery_timeout)
        battery_timeout--;
}


battery_status_e batt_status = STATUS_UNKNOWN;
battery_status_e Battery_Check (void)
{
    return batt_status;
}


unsigned short Battery_Get_Mains (void)
{
    return mains_input_voltage;
}


unsigned short Battery_Get_BatA (void)
{
    return batt_a_voltage;
}


unsigned short Battery_Get_BatB (void)
{
    return batt_b_voltage;
}


battery_states_e battery_state = START_12V_EXT_CONVERTION;
void Battery_Status (void)
{
    if (battery_timeout)
        return;

    battery_timeout = 50;    // take meas every 50ms
    
    switch (battery_state)
    {
    case START_12V_EXT_CONVERTION:
        AdcConvertChannel (Sense_12V_Ext);
        battery_state++;
        break;

    case WAIT_12V_EXT_CONVERTION:
        if (AdcConvertSingleChannelFinishFlag ())
        {
            mains_input_voltage = AdcConvertChannelResult ();

            if (mains_input_voltage > MAINS_VOLTAGE_MIN)
                batt_status = RUNNING_ON_MAINS;
            else
                batt_status = RUNNING_ON_BATTERY;                

            battery_state++;
        }
        break;

    case START_BAT_A_CONVERTION:
        AdcConvertChannel (Sense_Bat_A);
        battery_state++;
        break;

    case WAIT_BAT_A_CONVERTION:
        if (AdcConvertSingleChannelFinishFlag ())
        {
            batt_a_voltage = AdcConvertChannelResult ();
            battery_state++;
        }
        break;

    case START_BAT_B_CONVERTION:
        AdcConvertChannel (Sense_Bat_B);
        battery_state++;
        break;

    case WAIT_BAT_B_CONVERTION:
        if (AdcConvertSingleChannelFinishFlag ())
        {
            batt_b_voltage = AdcConvertChannelResult ();
            battery_state = START_12V_EXT_CONVERTION;
        }
        break;

    default:
        battery_state = START_12V_EXT_CONVERTION;
        break;
    }
}


void Battery_Convert_To_Volts (unsigned short adc_value,
                               unsigned char * v_int,
                               unsigned char * v_dec)
{
    // Rmult is 0.211
    // Vx = adc * 3.3 
    float fcalc = 1.0;

    fcalc = adc_value * 15.64;
    // fcalc = adc_value * 15.08;    // adjust 8.1 -> 7.81
    fcalc = fcalc / 4095.;

    *v_int = (unsigned char) fcalc;
    fcalc = fcalc - *v_int;
    fcalc = fcalc * 10;
    *v_dec = (unsigned char) fcalc;    
}


char Battery_Convert_Status_From_Adc (unsigned short adc_batt)
{
    char a = '0';
    
    if (adc_batt > BATT_ADC_3)
        a = '4';
    else if (adc_batt > BATT_ADC_2)
        a = '3';
    else if (adc_batt > BATT_ADC_1)
        a = '2';
    else if (adc_batt > BATT_ADC_0)
        a = '1';

    return a;
}
//--- end of file ---//
