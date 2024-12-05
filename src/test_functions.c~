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
#include "tim.h"
#include "bit_bang.h"

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
void TF_Act_Channels (void);
void TF_Enable_Channels (void);
void TF_Synchro_Channels (void);
void TF_Enable_5V_Comm (void);
void TF_Enable_Lcd (void);
void TF_Enable_Rpi (void);
void TF_Enable_Encoder (void);
void TF_Enable_Boost (void);


void TF_UsartChannel1_Loop (void);
void TF_UsartChannel2_Loop (void);
void TF_UsartChannel3_Loop (void);
void TF_UsartChannel4_Loop (void);

void TF_UsartRpi_Loop (void);
void TF_UsartRpi_String (void);

void TF_PowerOn_Channel1_Channel2 (void);

void TF_Tim6_Int (void);
void TF_Bit_Bang_Send (void);
void TF_Bit_Bang_Loop (void);

void TF_Int_Pb6 (void);
// void TF_Adc_Usart1_Tx (void);
// void TF_Adc_Usart1_Voltages (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Act_Channels ();
    // TF_Enable_Channels ();
    // TF_Synchro_Channels ();
    // TF_Enable_5V_Comm ();
    // TF_Enable_Lcd ();
    // TF_Enable_Rpi ();
    // TF_Enable_Encoder ();
    // TF_Enable_Boost ();

    // TF_UsartChannel1_Loop ();
    // TF_UsartChannel2_Loop ();
    // TF_UsartChannel3_Loop ();
    // TF_UsartChannel4_Loop ();    

    // TF_UsartRpi_Loop ();
    // TF_UsartRpi_String ();

    // TF_PowerOn_Channel1_Channel2 ();

    // TF_Adc_Usart1_Tx ();
    // TF_Adc_Usart1_Voltages ();

    // TF_Tim6_Int ();
    // TF_Bit_Bang_Send ();
    TF_Bit_Bang_Loop ();

    // TF_Int_Pb6();

}


void TF_Act_Channels (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ACT_PROBE_CH1)
        {
            ACT_PROBE_CH1_OFF;
            ACT_PROBE_CH2_OFF;
            ACT_PROBE_CH3_OFF;
            ACT_PROBE_CH4_OFF;            
        }
        else
        {
            ACT_PROBE_CH1_ON;
            ACT_PROBE_CH2_ON;
            ACT_PROBE_CH3_ON;
            ACT_PROBE_CH4_ON;            
        }
    }
}


void TF_Enable_Channels (void)
{
    while (1)
    {
        Wait_ms(1000);

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


void TF_Synchro_Channels (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (SYNC_CH1)
        {
            SYNC_CH1_OFF;
            SYNC_CH2_OFF;
            SYNC_CH3_OFF;
            SYNC_CH4_OFF;            
        }
        else
        {
            SYNC_CH1_ON;
            SYNC_CH2_ON;
            SYNC_CH3_ON;
            SYNC_CH4_ON;            
        }
    }
}


void TF_Enable_5V_Comm (void)
{
    while (1)
    {
        ENA_5V_COMM_ON;
        Wait_ms(5000);
        Act_Probe_Ch1_On();
        Wait_ms(15000);
        ENA_5V_COMM_OFF;
        Act_Probe_Ch1_Off();
        Wait_ms(20000);

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

        if (ENA_ENCODER)
        {
            ENA_ENCODER_OFF;
        }
        else
        {
            ENA_ENCODER_ON;
        }
    }
}


void TF_Enable_Boost (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ENA_BOOST)
        {
            ENA_BOOST_OFF;
        }
        else
        {
            ENA_BOOST_ON;
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


// place a shortcut Rx Tx on Rpi connector
void TF_UsartRpi_Loop (void)
{
    char buff [100];
    
    UsartRpiConfig ();
    
    while (1)
    {
        if (!timer_standby)
        {
            UsartRpiSend ("Mariano\n");
            timer_standby = 2000;
            if (SYNC_CH1)
                SYNC_CH1_OFF;
        }

        if (UsartRpiHaveData ())
        {
            UsartRpiHaveDataReset ();
            UsartRpiReadBuffer (buff, 100);
            if (strncmp(buff, "Mariano", sizeof("Mariano") - 1) == 0)
                SYNC_CH1_ON;
        }
    }
}


// Terminal Looping on Rpi connector
void TF_UsartRpi_String (void)
{
    char buff [100];

    UsartRpiConfig ();
    UsartRpiSend("rpi usart test... send a string:\n");
    
    while (1)
    {
        if (UsartRpiHaveData())
        {
            UsartRpiHaveDataReset();
            UsartRpiReadBuffer(buff, 100);

            Wait_ms(1000);

            int i = strlen(buff);
            if (i < 99)
            {
                buff[i] = '\n';
                buff[i+1] = '\0';
                UsartRpiSend(buff);
            }
        }
    }
}


void TF_PowerOn_Channel1_Channel2 (void)
{
    ENA_BOOST_ON;
    ENA_CH1_ON;
    ENA_CH2_ON;
    
    while (1);
}


// Bit Bang Tests Functions
void TF_Bit_Bang_Send (void)
{
    // char buff [100];
    Bit_Bang_Init ();
    timer_standby = 5000;
    
    while (1)
    {
        if (!timer_standby)
        {
            Bit_Bang_Tx_Send ("enc 0 p 1\n");
            // Bit_Bang_Tx_Send ("Ma");
            timer_standby = 1000;
        }
    }
}


void TF_Bit_Bang_Loop (void)
{
    char buff [100];

    Bit_Bang_Init ();
    Bit_Bang_Tx_Send ("\nRx 1200 test\n");
    
    while (1)
    {
        if (Bit_Bang_Rx_Have_Data())
        {
            unsigned char len = 0;
            // memset(buff, 0, 100);
            len = Bit_Bang_Rx_ReadBuffer(buff);

            Wait_ms(1000);

            if (len < 98)
            {
                buff[len] = '\n';
                buff[len + 1] = '\0';
                Bit_Bang_Tx_Send(buff);
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
            if (PB7)
                PB7_OFF;
            else
                PB7_ON;
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
            if (PB7)
                PB7_OFF;
            else
                PB7_ON;
        }
    }
}



//--- end of file ---//
