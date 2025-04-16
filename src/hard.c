//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.C #################################
//---------------------------------------------
#include "hard.h"
#include "stm32f10x.h"

#include "adc.h"
#include "tim.h"
#include "usart.h"

#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Ena_Rpi_On (void)
{
    ENA_RPI_ON;
}


void Ena_Ch1_On (void)
{
    ENA_CH1_ON;
}


void Ena_Ch2_On (void)
{
    ENA_CH2_ON;
}


void Ena_Ch3_On (void)
{
    ENA_CH3_ON;
}
    
void Ena_Ch4_On (void)
{
    ENA_CH4_ON;
}


unsigned char Ena_Ch1_Is_On (void)
{
    return ENA_CH1_ON;
}


unsigned char Ena_Ch2_Is_On (void)
{
    return ENA_CH2_ON;
}


unsigned char Ena_Ch3_Is_On (void)
{
    return ENA_CH3_ON;
}


unsigned char Ena_Ch4_Is_On (void)
{
    return ENA_CH4_ON;
}


void Ena_Rpi_Off (void)
{
    ENA_RPI_OFF;
}


void Ena_Ch1_Off (void)
{
    ENA_CH1_OFF;
}


void Ena_Ch2_Off (void)
{
    ENA_CH2_OFF;
}


void Ena_Ch3_Off (void)
{
    ENA_CH3_OFF;
}


void Ena_Ch4_Off (void)
{
    ENA_CH4_OFF;
}


// ch2 ch3 ch4 parallel
void Left_Off (void)
{
    IN_LEFT_CH2_OFF;
    IN_LEFT_CH3_OFF;
    IN_LEFT_CH4_OFF;    
}


void Left_On (void)
{
    IN_LEFT_CH2_ON;
    IN_LEFT_CH3_ON;
    IN_LEFT_CH4_ON;    
}


void Right_Off (void)
{
    IN_RIGHT_CH2_OFF;
    IN_RIGHT_CH3_OFF;
    IN_RIGHT_CH4_OFF;
}


void Right_On (void)
{
    IN_RIGHT_CH2_ON;
    IN_RIGHT_CH3_ON;
    IN_RIGHT_CH4_ON;
}


unsigned char Left_Is_On (void)
{
    return IN_LEFT_CH2;
}


void Sync_Ch1_On (void)
{
    SYNC_CH1_ON;
}


void Sync_Ch1_Off (void)
{
    SYNC_CH1_OFF;
}



void Hard_GetHardSoft (char * buff)
{
    sprintf(buff, "%s %s\n", HARD, SOFT);
}


void Hard_GetVoltages (char * buff)
{
    ADC_START;
    Wait_ms(1);

    sprintf(buff, "%d %d %d %d %d %d\n",
            SENSE_POWER,
            SENSE_MEAS,
            V_SENSE_28V,
            V_SENSE_25V,
            V_SENSE_11V,
            V_SENSE_8V);

}


void Hard_GetVoltages_Complete (void)
{
    char buff [50];
    int calc_int, calc_dec;
    
    ADC_START;
    Wait_ms(1);

    sprintf(buff, "%d %d %d %d %d %d\n",
            SENSE_POWER,
            SENSE_MEAS,
            V_SENSE_28V,
            V_SENSE_25V,
            V_SENSE_11V,
            V_SENSE_8V);

    Usart1Send (buff);

    // SENSE_POWER resistor multiplier 11
    calc_int = SENSE_POWER * 330 * 11;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "Power: %d.%02dV, ", calc_int, calc_dec);
    Usart1Send (buff);

    // SENSE_MEAS resistor multiplier 2
    calc_int = SENSE_MEAS * 330 * 2;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "Meas: %d.%02dV, ", calc_int, calc_dec);
    Usart1Send (buff);

    // V_SENSE_28V resistor multiplier 11
    calc_int = V_SENSE_28V * 330 * 11;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "V28: %d.%02dV, ", calc_int, calc_dec);
    Usart1Send (buff);

    // V_SENSE_25V resistor multiplier 11
    calc_int = V_SENSE_25V * 330 * 11;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "V25: %d.%02dV, ", calc_int, calc_dec);
    Usart1Send (buff);

    // V_SENSE_11V resistor multiplier 11
    calc_int = V_SENSE_11V * 330 * 11;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "V11: %d.%02dV, ", calc_int, calc_dec);
    Usart1Send (buff);

    // V_SENSE_8V resistor multiplier 11
    calc_int = V_SENSE_8V * 330 * 11;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "V8: %d.%02dV\n", calc_int, calc_dec);
    Usart1Send (buff);
    
}


void Speaker_On (void)
{
    ENA_SPEAKER_ON;
}


void Speaker_Off (void)
{
    ENA_SPEAKER_OFF;
}
//--- end of file ---//
