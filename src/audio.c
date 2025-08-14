//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### AUDIO.C ################################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "hard.h"
#include "audio.h"
#include "tim.h"
#include "timer_signals.h"
#include "dac.h"

#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
const short audio_sine_table [] = {783,1447,1891,2047,1891,1447,783,0,
                                   -783,-1447,-1891,-2047,-1891,-1447,-783,0,};

const unsigned short audio_dac_offset = 2047;
volatile unsigned char signal_cnt = 0;
volatile unsigned short cycle_cnt_int = 0;
volatile unsigned short cycle_cnt = 0;

// volume starts at 100% 6 / 16
volatile unsigned char audio_volume_mult = 6;
volatile unsigned char audio_volume_bit_shift = 4;

// Module Private Functions ----------------------------------------------------
void Audio_Ampli_Enable (void);
void Audio_Ampli_Disable (void);
void Audio_Timer_Start (void);
void Audio_Timer_Stop (void);


// Module Functions ------------------------------------------------------------
void Audio_Init (void)
{
    DAC_Output2(audio_dac_offset);
    Audio_Timer_Stop ();
    Audio_Ampli_Disable ();    
}


void Audio_Start_Tone (unsigned short time_ms, unsigned short freq_hz)
{
    signal_cnt = 0;
    cycle_cnt_int = 0;

    unsigned int calc = freq_hz * time_ms / 1000;
    cycle_cnt = (unsigned short) calc;

    calc = 1000000;    //1e6 / (freq_hz * 16)
    calc = calc / (freq_hz * 16);

    Audio_Timer_Set_Params (calc);
    Audio_Timer_Start();
    Audio_Ampli_Enable ();
}


void Audio_Volume_Set (unsigned char perc)
{
    if (perc > 90)
    {
	// 6 / 16 = 0.375
	audio_volume_mult = 6;
	audio_volume_bit_shift = 4;
    }
    else if (perc > 80)
    {
	// 5 / 16 = 0.3125
	audio_volume_mult = 5;
	audio_volume_bit_shift = 4;	
    }
    else if (perc > 70)
    {
	// 4 / 16 = 0.2625
	audio_volume_mult = 4;
	audio_volume_bit_shift = 4;	
    }
    else if (perc > 60)
    {
	// 2 / 8 = 0.25
	audio_volume_mult = 2;
	audio_volume_bit_shift = 3;	
    }
    else if (perc > 50)
    {
	// 3 / 16 = 0.1875
	audio_volume_mult = 3;
	audio_volume_bit_shift = 4;	
    }
    else if (perc > 40)
    {
	// 2 / 16 = 0.125
	audio_volume_mult = 2;
	audio_volume_bit_shift = 4;	
    }
    else if (perc > 30)
    {
	// 1 / 16 = 0.0625
	audio_volume_mult = 1;
	audio_volume_bit_shift = 4;	
    }
    // else if (perc > 20)
    // {
    // 	// 3 / 16 = 0.1875
    // 	audio_volume_mult = 3;
    // 	audio_volume_bit_shift = 4;	
    // }
    // else if (perc > 10)
    // {
    // 	// 3 / 16 = 0.1875
    // 	audio_volume_mult = 3;
    // 	audio_volume_bit_shift = 4;	
    // }
    else    // audio off
    {
	audio_volume_mult = 0;
	audio_volume_bit_shift = 0;	
    }    
}


void Audio_Timer_Interrupt_Handler (void)
{
    if (signal_cnt < 15)
    {
	short sample = audio_sine_table[signal_cnt] * audio_volume_mult;
	sample >>= audio_volume_bit_shift;
	DAC_Output2(sample + audio_dac_offset);
	// DAC_Output2(audio_sine_table[signal_cnt] + audio_dac_offset);
	signal_cnt++;
    }
    else if (cycle_cnt_int < cycle_cnt)
    {
	short sample = audio_sine_table[signal_cnt] * 6;
	sample >>= 4;
	DAC_Output2(sample + audio_dac_offset);	
	// DAC_Output2(audio_sine_table[signal_cnt] + audio_dac_offset);
	cycle_cnt_int++;
	signal_cnt = 0;
    }
    else
    {
	DAC_Output2(audio_dac_offset);
	Audio_Timer_Stop();
	Audio_SM (AUDIO_TONE_END_EVENT);
	// Audio_Ampli_Disable ();
    }
}


typedef enum {
    AUDIO_RESET,
    AUDIO_FIRST_TONE_UP,
    AUDIO_FIRST_TONE_DWN,
    AUDIO_SECOND_TONE

} audio_sm_state_e;

audio_sm_state_e audio_sm_state = AUDIO_RESET;
void Audio_SM (audio_sm_event_e new_event)
{
    if (new_event == AUDIO_RESET_EVENT)
    {
	audio_sm_state = AUDIO_RESET;
	Audio_Timer_Stop();
	Audio_Ampli_Disable ();
	return;
    }
    
    switch (audio_sm_state)
    {
    case AUDIO_RESET:
	if (audio_volume_mult == 0)    // no audio volume
	    break;
	    
	if (new_event == AUDIO_START_UP_EVENT)
	{
	    Audio_Start_Tone (100, 400);
	    // Audio_Start_Tone (100, 500);	    
	    audio_sm_state = AUDIO_FIRST_TONE_UP;
	}

	if (new_event == AUDIO_START_DWN_EVENT)
	{
	    Audio_Start_Tone (100, 600);
	    // Audio_Start_Tone (100, 800);	    
	    audio_sm_state = AUDIO_FIRST_TONE_DWN;
	}
	break;

    case AUDIO_FIRST_TONE_UP:
	if (new_event == AUDIO_TONE_END_EVENT)
	{
	    Audio_Start_Tone (100, 600);
	    // Audio_Start_Tone (100, 800);	    
	    audio_sm_state = AUDIO_SECOND_TONE;
	}
	break;

    case AUDIO_FIRST_TONE_DWN:
	if (new_event == AUDIO_TONE_END_EVENT)
	{
	    Audio_Start_Tone (100, 400);
	    // Audio_Start_Tone (100, 500);	    
	    audio_sm_state = AUDIO_SECOND_TONE;
	}
	break;

    case AUDIO_SECOND_TONE:
	if (new_event == AUDIO_TONE_END_EVENT)
	{
	    audio_sm_state = AUDIO_RESET;
	    Audio_Timer_Stop();
	    Audio_Ampli_Disable ();
	}
	break;
    }
}


void Audio_Ampli_Enable (void)
{
    Speaker_On();
}


void Audio_Ampli_Disable (void)
{
    Speaker_Off();
}


void Audio_Timer_Start (void)
{
    TIM5_Start();
}


void Audio_Timer_Stop (void)
{
    TIM5_Stop();
}


void Audio_Timer_Set_Params (unsigned short new_arr)
{
    TIM5_Set_Arr(new_arr);
}
//--- end of file ---//
