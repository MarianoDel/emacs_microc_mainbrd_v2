//-----------------------------------------------------------
// #### MICROCURRENT CHANNEL PROJECT F103 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TREATMENT.C #########################################
//-----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "treatment.h"
#include "comms.h"
#include "hard.h"
#include "usart_channels.h"
#include "usart.h"
#include "tim.h"
#include "dac.h"

#include "signals.h"
#include "utils.h"
#include "timer_signals.h"
#include "audio.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// Private Types Constants and Macros ------------------------------------------
typedef enum {
    TREATMENT_INIT = 0,
    TREATMENT_STANDBY,
    TREATMENT_SQUARE_RUNNING,
    TREATMENT_SINE_RUNNING,
    TREATMENT_STOPPING

} treatment_e;


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
treatment_conf_t treat_conf_sine;
treatment_conf_t treat_conf_square;
treatment_e treat_state = 0;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Treatment_Manager (void)
{
    switch (treat_state)
    {
    case TREATMENT_INIT:
        Signals_Stop ();
	Timer_Polarity (POLARITY_NEG);    // or null polarity
        treat_state++;
        break;
                
    case TREATMENT_STANDBY:
        
        if (Treatment_Sine_Start_Flag ())
        {
            Treatment_Sine_Start_Flag_Reset ();
	    UsartRpiSend("starting sinusoidal\r\n");
	    Signals_Sinusoidal_Reset ();
	    treat_state = TREATMENT_SINE_RUNNING;
	    Audio_SM(AUDIO_START_UP_EVENT);
        }

        if (Treatment_Square_Start_Flag ())
        {
            Treatment_Square_Start_Flag_Reset ();
	    UsartRpiSend("starting square\r\n");

	    char lbuf [40];
	    sprintf(lbuf, "  flag: %d\r\n", Treatment_Get_Flag());
	    UsartRpiSend(lbuf);
	    
	    Signals_Square_Reset ();
	    treat_state = TREATMENT_SQUARE_RUNNING;
	    Audio_SM(AUDIO_START_UP_EVENT);	    
	}	

        break;
        
    case TREATMENT_SQUARE_RUNNING:
        Signals_Square (&treat_conf_square);

        if (Treatment_Stop_Flag ())
        {
            Treatment_Stop_Flag_Reset ();
            treat_state = TREATMENT_STOPPING;
        }
        break;
        
    case TREATMENT_SINE_RUNNING:
        Signals_Sinusoidal (&treat_conf_sine);

        if (Treatment_Stop_Flag ())
        {
            Treatment_Stop_Flag_Reset ();            
            treat_state = TREATMENT_STOPPING;
        }
        break;
                
    case TREATMENT_STOPPING:
        Signals_Stop ();

        UsartRpiSend("stopped\r\n");
	Audio_SM(AUDIO_START_DWN_EVENT);

	Timer_Polarity (POLARITY_NEG);    // or null polarity
	treat_state = TREATMENT_STANDBY;
        break;

    default:
        treat_state = TREATMENT_INIT;
        break;
    }

}


resp_e Treatment_SetFrequency_Str (mode_e mode, char * str)
{
    resp_e resp = resp_error;
    int figures = 0;
    unsigned short new_freq_int = 0;
    unsigned short new_freq_dec = 0;    
    
    // what we get is E.DD | EE.DD | EEE.DD, always two decimal positions
    figures = StringIsANumber(str, &new_freq_int);

    if ((figures) && (figures <= 3) && (new_freq_int <= 999))
    {
        str += figures + 1;    // point normalization
        figures = StringIsANumber(str, &new_freq_dec);

        if ((figures > 1) && (figures < 3) && (new_freq_dec <= 99))
        {
            resp = Treatment_SetFrequency (mode, new_freq_int, new_freq_dec);
        }
    }

    return resp;
}


resp_e Treatment_SetFrequency (mode_e mode, unsigned short fint, unsigned short fdec)
{
    resp_e resp = resp_error;
    
    if ((fint <= 999) && (fdec <= 99))
    {
	if (mode == MODE_SINE)
	{
	    treat_conf_sine.freq_int = fint;
	    treat_conf_sine.freq_dec = fdec;
	}
	else
	{
	    treat_conf_square.freq_int = fint;
	    treat_conf_square.freq_dec = fdec;
	}
	
        Signals_Set_Frequency_Intensity_Change_Flag ();
        
        resp = resp_ok;
    }

    return resp;
}


resp_e Treatment_SetIntensity_Str (mode_e mode, char * str)
{
    resp_e resp = resp_error;
    int figures = 0;
    unsigned short new_intensity = 0;
    
    // what we get is E | EE | EEE, no decimal positions
    figures = StringIsANumber(str, &new_intensity);

    if ((figures) && (figures <= 3) && (new_intensity <= 999))
    {
        resp = Treatment_SetIntensity (mode, new_intensity);
    }

    return resp;
}


resp_e Treatment_SetIntensity (mode_e mode, unsigned short intensity)
{
    resp_e resp = resp_error;
    
    if (intensity <= 999)
    {
	if (mode == MODE_SINE)
	    treat_conf_sine.intensity = intensity;
	else
	    treat_conf_square.intensity = intensity;

        Signals_Set_Frequency_Intensity_Change_Flag ();
        
        resp = resp_ok;
    }

    return resp;
}


resp_e Treatment_SetPolarity_Str (char * str)
{
    resp_e resp = resp_error;
    
    if (!strncmp(str, "pos", sizeof("pos") - 1))
    {
        treat_conf_sine.polarity = POLARITY_POS;
        treat_conf_square.polarity = POLARITY_POS;	
        resp = resp_ok;
    }
    else if (!strncmp(str, "neg", sizeof("neg") - 1))
    {
        treat_conf_sine.polarity = POLARITY_NEG;
        treat_conf_square.polarity = POLARITY_NEG;	
        resp = resp_ok;        
    }
    else if (!strncmp(str, "alt", sizeof("alt") - 1))
    {
        treat_conf_sine.polarity = POLARITY_ALT;
        treat_conf_square.polarity = POLARITY_ALT;	
        resp = resp_ok;        
    }

    if (resp == resp_ok)
        Signals_Set_Frequency_Intensity_Change_Flag ();
    
    return resp;
}


// resp_e Treatment_SetMode_Str (char * str)
// {
//     resp_e resp = resp_error;
    
//     if (!strncmp(str, "square", sizeof("square") - 1))
//     {
//         treatment_conf.mode = MODE_SQUARE;
//         resp = resp_ok;        
//     }
//     else if (!strncmp(str, "sine", sizeof("sine") - 1))
//     {
//         treatment_conf.mode = MODE_SINE;
//         resp = resp_ok;        
//     }

//     return resp;
// }


resp_e Treatment_SetThreshold_Str (char * str)
{
    resp_e resp = resp_error;
    int figures = 0;
    unsigned short new_threshold = 0;
    
    // what we get is E | EE | EEE, no decimal positions
    figures = StringIsANumber(str, &new_threshold);

    if ((figures) && (figures <= 3) && (new_threshold <= 999))
    {
        resp = Treatment_SetThreshold (new_threshold);
    }

    return resp;
}


resp_e Treatment_SetThreshold (unsigned short threshold)
{
    resp_e resp = resp_error;
    
    if (threshold <= 100)
    {
        treat_conf_sine.threshold = threshold;
        treat_conf_square.threshold = threshold;	
        resp = resp_ok;
    }

    return resp;
}


void Treatment_Start (mode_e mode)
{
    if (mode == MODE_SINE)
	Treatment_Sine_Start_Flag_Set ();
    else
	Treatment_Square_Start_Flag_Set ();
}


void Treatment_Stop (void)
{
    Treatment_Stop_Flag_Set ();
}


// resp_e Treatment_SetGain_Str (char * str)
// {
//     resp_e resp = resp_error;
//     int figures = 0;
//     unsigned short new_gain = 0;
    
//     // what we get is E | EE | EEE, no decimal positions
//     figures = StringIsANumber(str, &new_gain);

//     if ((figures) && (figures <= 3) && (new_gain <= 999))
//     {
//         resp = Treatment_SetGain (new_gain);
//     }

//     return resp;
// }


// resp_e Treatment_SetGain (unsigned short gain)
// {
//     resp_e resp = resp_error;
    
//     if (gain <= 100)
//     {
//         treatment_conf.gain = gain;
//         Meas_Square_Set_Dac_Gain(gain);
//         resp = resp_ok;
//     }

//     return resp;
// }


// unsigned char Treatment_GetGain (void)
// {
//     return treatment_conf.gain;
// }


void Treatment_GetAllConf (char * tosend)
{
    // char buf[30];

    // // Selected signal
    // sprintf(buf, "signal: %d\n", treatment_conf.treatment_signal.signal);
    // strcpy(tosend, buf);

    // // Selected Freq
    // sprintf(buf, "freq: %d.%02dHz\n",
    //         treatment_conf.treatment_signal.freq_int,
    //         treatment_conf.treatment_signal.freq_dec);
    // strcat(tosend, buf);

    // // Selected power
    // sprintf(buf, "power: %d\n", treatment_conf.treatment_signal.power);
    // strcat(tosend, buf);

    // // Treatment duration
    // sprintf(buf, "time in s: %d\n", treatment_conf.treatment_time);
    // strcat(tosend, buf);

    // // Active channels
    // // unsigned char c1 = 0;
    // // unsigned char c2 = 0;
    // // unsigned char c3 = 0;

    // // if (treatment_conf.channels_in_treatment & ENABLE_CH1_FLAG)
    // //     c1 = 1;

    // // if (treatment_conf.channels_in_treatment & ENABLE_CH2_FLAG)
    // //     c2 = 1;

    // // if (treatment_conf.channels_in_treatment & ENABLE_CH3_FLAG)
    // //     c3 = 1;
    
    // // sprintf(buf, "in treat ch1:%d ch2:%d ch3:%d\n", c1, c2, c3);
    // // strcat(tosend, buf);

    // // //Timer for sync
    // // unsigned short tsync = 0;
    // // // tsync = treatment_conf.timer_synchro / 10;
    // // sprintf(buf, "synchro: %dms\n", tsync);
    // // strcat(tosend, buf);
}


#define START_SINE_SET    0x01
#define START_SINE_SET_MASK    0xFE
#define START_SQUARE_SET    0x02
#define START_SQUARE_SET_MASK    0xFD
#define STOP_SET    0x04
#define STOP_SET_MASK    0xFB
unsigned char start_stop_flag = 0;
unsigned char Treatment_Sine_Start_Flag (void)
{
    if (start_stop_flag & START_SINE_SET)
        return 1;

    return 0;
}


void Treatment_Sine_Start_Flag_Set (void)
{
    start_stop_flag |= START_SINE_SET;
}


void Treatment_Sine_Start_Flag_Reset (void)
{
    start_stop_flag &= START_SINE_SET_MASK;
}


unsigned char Treatment_Square_Start_Flag (void)
{
    if (start_stop_flag & START_SQUARE_SET)
        return 1;

    return 0;
}


void Treatment_Square_Start_Flag_Set (void)
{
    start_stop_flag |= START_SQUARE_SET;
}


void Treatment_Square_Start_Flag_Reset (void)
{
    start_stop_flag &= START_SQUARE_SET_MASK;
}


unsigned char Treatment_Stop_Flag (void)
{
    if (start_stop_flag & STOP_SET)
        return 1;

    return 0;
}


void Treatment_Stop_Flag_Set (void)
{
    start_stop_flag |= STOP_SET;
}


void Treatment_Stop_Flag_Reset (void)
{
    start_stop_flag &= STOP_SET_MASK;
}


unsigned char Treatment_Get_Flag (void)
{
    return start_stop_flag;
}
//--- end of file ---//
