//----------------------------------------------------------
// #### MAGNET PROJECT - Custom Board ####
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C ###################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "hard.h"
#include "stm32f10x.h"
#include "gpio.h"
#include "adc.h"
#include "usart_channels.h"
#include "usart.h"
#include "dma.h"
#include "dac.h"
#include "tim.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];
extern volatile unsigned short wait_ms_var;
extern volatile unsigned short timer_standby;
extern volatile unsigned char timer_6_uif_flag;
extern volatile unsigned char rx_int_handler;
// extern volatile unsigned char usart3_have_data;


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Enable_Channels (void);
void TF_Synchro_Channel1 (void);
void TF_Synchro_Dac (void);
void TF_Disable_Ch3_Ch4 (void);

void TF_Enable_Lcd (void);
void TF_Enable_Rpi (void);
void TF_Enable_Encoder (void);

void TF_UsartChannel1_Loop (void);
void TF_UsartChannel2_Loop (void);
void TF_UsartChannel3_Loop (void);
void TF_UsartChannel4_Loop (void);

void TF_UsartRpi_Tx (void);
void TF_UsartRpi_String (void);

void TF_PowerOn_Channel1_Channel2 (void);

void TF_Tim6_Int (void);
// void TF_Adc_Usart1_Tx (void);
// void TF_Adc_Usart1_Voltages (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Enable_Channels ();
    // TF_Synchro_Channel1 ();
    // TF_Synchro_Dac ();
    TF_Disable_Ch3_Ch4 ();
    
    // TF_Enable_Lcd ();
    // TF_Enable_Rpi ();
    // TF_Enable_Encoder ();

    // TF_UsartChannel1_Loop ();
    // TF_UsartChannel2_Loop ();
    // TF_UsartChannel3_Loop ();
    // TF_UsartChannel4_Loop ();    

    // TF_UsartRpi_Tx ();
    // TF_UsartRpi_String ();

    
    // TF_PowerOn_Channel1_Channel2 ();

    // TF_Adc_Usart1_Tx ();
    // TF_Adc_Usart1_Voltages ();

    // TF_Tim6_Int ();

    // TF_Int_Pb6();

}


void TF_Disable_Ch3_Ch4 (void)
{
    Ena_Ch1_Off();
    Ena_Ch2_Off();
    Ena_Ch3_Off();
    Ena_Ch4_Off();
    
    while (1)
    {
        Wait_ms(5000);
	Ena_Ch3_On();
	Ena_Ch4_On();	
        Wait_ms(5000);	
	Ena_Ch3_Off();
	Ena_Ch4_Off();	
    }
}


void TF_Enable_Channels (void)
{
    while (1)
    {
        Wait_ms(8000);

        if (ENA_CH1)
        {
            ENA_CH1_OFF;
            ENA_CH2_OFF;
            ENA_CH3_OFF;
            ENA_CH4_OFF;            
        }
        else
        {
            ENA_CH1_ON;
            ENA_CH2_ON;
            ENA_CH3_ON;
            ENA_CH4_ON;            
        }
    }
}


void TF_Synchro_Channel1 (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (SYNC_CH1)
        {
            SYNC_CH1_OFF;
        }
        else
        {
            SYNC_CH1_ON;
        }
    }
}


void TF_Synchro_Dac (void)
{
    Ena_Ch1_Off();
    Ena_Ch2_Off();
    Ena_Ch3_Off();
    Ena_Ch4_Off();

    DAC_Config ();
    
    while (1)
    {
	Sync_Ch1_On();
	DAC_Output1 (2047);
	DAC_Output2 (4095);
	Wait_ms(200);

	Sync_Ch1_Off();
	DAC_Output1 (4095);
	DAC_Output2 (2047);
	Wait_ms(200);	
    }
}


void TF_Enable_Lcd (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ENA_LCD)
        {
            ENA_LCD_OFF;
        }
        else
        {
            ENA_LCD_ON;
        }
    }
}


void TF_Enable_Rpi (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ENA_RPI)
        {
            ENA_RPI_OFF;
        }
        else
        {
            ENA_RPI_ON;
        }
    }
}


void TF_Enable_Encoder (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ENA_ENC_CONN)
        {
            ENA_ENC_CONN_OFF;
        }
        else
        {
            ENA_ENC_CONN_ON;
        }
    }
}


// place a shortcut on IC4 2 & IC3 4
void TF_UsartChannel1_Loop (void)
{
    char buff [100];
    
    UsartChannel1Config();
    
    while (1)
    {
        if (!timer_standby)
        {
            UsartChannel1Send("Mariano\n");
            timer_standby = 2000;
            if (SYNC_CH1)
                SYNC_CH1_OFF;
        }

        if (UsartChannel1HaveData())
        {
            UsartChannel1HaveDataReset();
            UsartChannel1ReadBuffer(buff, 100);
            if (strncmp(buff, "Mariano", sizeof("Mariano") - 1) == 0)
                SYNC_CH1_ON;
        }
    }
}


// place a shortcut on IC4 2 & IC3 4
void TF_UsartChannel2_Loop (void)
{
    char buff [100];
    
    UsartChannel2Config();
    
    while (1)
    {
        if (!timer_standby)
        {
            UsartChannel2Send("Mariano\n");
            timer_standby = 2000;
            if (SYNC_CH1)
                SYNC_CH1_OFF;
        }

        if (UsartChannel2HaveData())
        {
            UsartChannel2HaveDataReset();
            UsartChannel2ReadBuffer(buff, 100);
            if (strncmp(buff, "Mariano", sizeof("Mariano") - 1) == 0)
                SYNC_CH1_ON;
        }
    }
}


// place a shortcut on IC4 2 & IC3 4
void TF_UsartChannel3_Loop (void)
{
    char buff [100];
    
    UsartChannel3Config();
    
    while (1)
    {
        if (!timer_standby)
        {
            UsartChannel3Send("Mariano\n");
            timer_standby = 2000;
            if (SYNC_CH1)
                SYNC_CH1_OFF;
        }

        if (UsartChannel3HaveData())
        {
            UsartChannel3HaveDataReset();
            UsartChannel3ReadBuffer(buff, 100);
            if (strncmp(buff, "Mariano", sizeof("Mariano") - 1) == 0)
                SYNC_CH1_ON;
        }
    }
}


// place a shortcut on IC4 2 & IC3 4
void TF_UsartChannel4_Loop (void)
{
    char buff [100];
    
    UsartChannel4Config();
    
    while (1)
    {
        if (!timer_standby)
        {
            UsartChannel4Send("Mariano\n");
            timer_standby = 2000;
            if (SYNC_CH1)
                SYNC_CH1_OFF;
        }

        if (UsartChannel4HaveData())
        {
            UsartChannel4HaveDataReset();
            UsartChannel4ReadBuffer(buff, 100);
            if (strncmp(buff, "Mariano", sizeof("Mariano") - 1) == 0)
                SYNC_CH1_ON;
        }
    }
}


// Sending loop data
void TF_UsartRpi_Tx (void)
{
    Ena_Ch1_Off();
    Ena_Ch2_Off();
    Ena_Ch3_Off();
    Ena_Ch4_Off();

    Ena_Rpi_On();
    Wait_ms(500);
    
    UsartRpiConfig ();
    UsartRpiSend("tx rpi usart test... transmitt every two seconds\r\n"); 
    Wait_ms(2000);
    
    while (1)
    {
	SYNC_CH1_ON;
	UsartRpiSend("Mariano\r\n");
	Wait_ms(300);
	SYNC_CH1_OFF;	
	Wait_ms(1700);
    }
}


// Terminal Looping on Rpi connector
void TF_UsartRpi_String (void)
{
    char buff [100];

    Ena_Ch1_Off();
    Ena_Ch2_Off();
    Ena_Ch3_Off();
    Ena_Ch4_Off();

    Ena_Rpi_On();
    Wait_ms(500);
    
    UsartRpiConfig ();
    UsartRpiSend("rpi usart test... send a string:\r\n");
    
    while (1)
    {
        if (UsartRpiHaveData())
        {
	    SYNC_CH1_ON;
            UsartRpiHaveDataReset();
            UsartRpiReadBuffer(buff, 100);

            Wait_ms(1000);
	    SYNC_CH1_OFF;	    

            int i = strlen(buff);
            if (i < 100 - 3)
            {
                buff[i] = '\r';
                buff[i+1] = '\n';		
                buff[i+2] = '\0';
                UsartRpiSend(buff);
            }
        }
    }
}


void TF_Tim6_Int (void)
{
    // char buff [100];
    TIM6_Init ();
    TIM6_Start();
    // Bit_Bang_Init ();
    
    while (1)
    {
        if (timer_6_uif_flag)
        {
            timer_6_uif_flag = 0;
        }
    }
}


void TF_Int_Pb6 (void)
{
    Wait_ms(1000);
    
    EXTIOn();
    
    while (1)
    {
        if (rx_int_handler)
        {
            rx_int_handler = 0;
        }
    }
}



//--- end of file ---//
