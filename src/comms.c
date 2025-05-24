//---------------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS.C ###########################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comms.h"
#include "answers_defs.h"
#include "hard.h"
// #include "adc.h"

#include "usart_channels.h"
#include "usart.h"
#include "treatment.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Module Private Types Constants and Macros -----------------------------------
char s_ans_ok [] = {"ok\r\n"};
char s_ans_nok [] = {"nok\r\n"};
#define SIZEOF_LOCAL_BUFF    128
#define COMMS_TT_RELOAD    3000


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
char local_buff [SIZEOF_LOCAL_BUFF];
volatile unsigned short comms_timeout = 0;
    

// Module Private Functions ----------------------------------------------------
static void Comms_Messages (char * msg_str);
static void Comms_Messages_For_Channels_Treatment (char * msg_str);
static void Comms_Bridge_Channel1_Msg (char * msg_for_ch1);
void Comms_Bridge_Conn_Msg (char * local_buff);
static void Comms_Bridge_Rpi_With_Connectors (void);


// Module Functions ------------------------------------------------------------
void Comms_Timeouts (void)
{
    if (comms_timeout)
        comms_timeout--;
}

#ifdef RPI_CONN_ONLY_ONE
unsigned char rpi_answers = 0;
unsigned char Comms_Rpi_Answering (void)
{
    if (rpi_answers)
	return 1;
    else if (comms_timeout)
    {
	rpi_answers = 1;
        return 1;
    }
    
    return 0;
}
#endif

#ifdef RPI_CONN_BY_TIMER
unsigned char Comms_Rpi_Answering (void)
{
    if (comms_timeout)
        return 1;
    
    return 0;
}
#endif

void Comms_Update (void)
{
    if (UsartRpiHaveData())
    {
        UsartRpiHaveDataReset();
        UsartRpiReadBuffer(local_buff, SIZEOF_LOCAL_BUFF);
        Comms_Messages(local_buff);

        comms_timeout = COMMS_TT_RELOAD;
    }
}


static void Comms_Messages (char * msg_str)
{
    // check if its own, broadcast or channel
    if (strncmp (msg_str, "ch", sizeof("ch") - 1) == 0)
    {
        // check enable or disable
        if (strncmp ((msg_str + 4), "enable", sizeof("enable") - 1) == 0)
        {
	    switch (*(msg_str + 2))
	    {
	    case '1':
		Ena_Ch1_On();
		break;

	    case '2':
		Ena_Ch2_On();
		break;

	    case '3':
		Ena_Ch3_On();		
		break;

	    case '4':
		Ena_Ch4_On();		
		break;

	    case 'f':
		Ena_Ch1_On();
		Ena_Ch2_On();
		Ena_Ch3_On();
		Ena_Ch4_On();
		break;
	    }
            UsartRpiSend(s_ans_ok);
        }
        else if (strncmp ((msg_str + 4), "disable", sizeof("disable") - 1) == 0)
        {
	    switch (*(msg_str + 2))
	    {
	    case '1':
		Ena_Ch1_Off();
		break;

	    case '2':
		Ena_Ch2_Off();
		break;
		
	    case '3':
		Ena_Ch3_Off();		
		break;

	    case '4':
		Ena_Ch4_Off();		
		break;

	    case 'f':
		Ena_Ch1_Off();
		Ena_Ch2_Off();
		Ena_Ch3_Off();
		Ena_Ch4_Off();
		break;
	    }
            UsartRpiSend(s_ans_ok);
        }
    }
    
    else if (strncmp (msg_str, "encod", sizeof("encod") - 1) == 0)
    {
        char * pmsg = msg_str + sizeof("encod") - 1;
        
        // check enable if valid number
        if ((*pmsg >= '0') && (*pmsg <= '7'))
        {
            if ((*(pmsg + 1) == ' ') &&
                (*(pmsg + 2) >= '0') &&
                (*(pmsg + 2) <= ';'))
            {
                char buff_encod [30] = { 0 };
                // "enc 0 1\n"
                sprintf(buff_encod, "enc %c %c\n", *(pmsg + 0), *(pmsg + 2));
                // Bit_Bang_Tx_Send(buff_encod);
                // Bit_Bang_Tx_Send("enc 0 1\n");                
                // i2c_driver_set_encod (*pmsg - '0', *(pmsg + 2) - '0');
            }
        }
    }
    
    else if (strncmp (msg_str, "conn", sizeof("conn") - 1) == 0)
    {
	Comms_Bridge_Conn_Msg (msg_str);
    }

    else if (strncmp (msg_str, "bridge conn", sizeof("bridge conn") - 1) == 0)
    {
	Comms_Bridge_Rpi_With_Connectors ();
    }
    
    else if (strncmp (msg_str, "sup", sizeof("sup") - 1) == 0)
    {
        // not implemented yet!
        UsartRpiSend(s_ans_ok);
    }
    
    else
    {
	// other messages
	Comms_Messages_For_Channels_Treatment (msg_str);
	// UsartRpiSend(s_ans_nok);
    }    
}


void Comms_Messages_For_Channels_Treatment (char * msg_str)
{
    resp_e resp;
    
    char * msg = msg_str;
    
    // -- config messages for signals --
    if (!strncmp(msg, "sine frequency", sizeof("sine frequency") - 1))
    {
        resp = Treatment_SetFrequency_Str (MODE_SINE, msg + sizeof("sine frequency"));
        if (resp == resp_ok)
	{
	    Comms_Bridge_Channel1_Msg (msg);
            UsartRpiSend (s_ans_ok);
	}
        else
            UsartRpiSend (s_ans_nok);
    }

    if (!strncmp(msg, "square frequency", sizeof("square frequency") - 1))
    {
        resp = Treatment_SetFrequency_Str (MODE_SQUARE, msg + sizeof("square frequency"));
        if (resp == resp_ok)
	{
	    Comms_Bridge_Channel1_Msg (msg);
            UsartRpiSend (s_ans_ok);
	}
        else
            UsartRpiSend (s_ans_nok);
    }
    
    else if (!strncmp(msg, "sine intensity", sizeof("sine intensity") - 1))
    {
        resp = Treatment_SetIntensity_Str (MODE_SINE, msg + sizeof("sine intensity"));
        if (resp == resp_ok)
	{
	    Comms_Bridge_Channel1_Msg (msg);
            UsartRpiSend (s_ans_ok);
	}
        else
            UsartRpiSend (s_ans_nok);
    }

    else if (!strncmp(msg, "square intensity", sizeof("square intensity") - 1))
    {
        resp = Treatment_SetIntensity_Str (MODE_SQUARE, msg + sizeof("square intensity"));
        if (resp == resp_ok)
	{
	    Comms_Bridge_Channel1_Msg (msg);
            UsartRpiSend (s_ans_ok);
	}
        else
            UsartRpiSend (s_ans_nok);
    }
    
    else if (!strncmp(msg, "polarity", sizeof("polarity") - 1))
    {
        resp = Treatment_SetPolarity_Str (msg + sizeof("polarity"));
        if (resp == resp_ok)
	{
	    Comms_Bridge_Channel1_Msg (msg);
	    Comms_Bridge_Conn_Msg (msg);
            UsartRpiSend (s_ans_ok);
	}
        else
            UsartRpiSend (s_ans_nok);
    }

    // config messages for channel setup
    // else if (!strncmp(msg, "mode", sizeof("mode") - 1))
    // {
    //     resp = Treatment_SetMode_Str (msg + sizeof("mode"));
    //     if (resp == resp_ok)
    // 	{
    // 	    Comms_Bridge_Channel1_Msg (msg);
    //         UsartRpiSend (s_ans_ok);
    // 	}
    //     else
    //         UsartRpiSend (s_ans_nok);
    // }

    else if (!strncmp(msg, "threshold", sizeof("threshold") - 1))
    {
        resp = Treatment_SetThreshold_Str (msg + sizeof("threshold"));
        if (resp == resp_ok)
	{
	    Comms_Bridge_Channel1_Msg (msg);
            UsartRpiSend (s_ans_ok);
	}
        else
            UsartRpiSend (s_ans_nok);
    }

    // -- operation messages --
    else if (!strncmp(msg, "square start", sizeof("square start") - 1))
    {
        Treatment_Start (MODE_SQUARE);
	Comms_Bridge_Channel1_Msg (msg);
        UsartRpiSend (s_ans_ok);
    }

    else if (!strncmp(msg, "sine start", sizeof("sine start") - 1))
    {
        Treatment_Start (MODE_SINE);
	Comms_Bridge_Channel1_Msg (msg);
        UsartRpiSend (s_ans_ok);
    }

    // else if (!strncmp(msg, "start", sizeof("start") - 1))
    // {
    //     Treatment_Start ();
    //     UsartRpiSend (s_ans_ok);
    // }
    
    else if (!strncmp(msg, "stop", sizeof("stop") - 1))
    {
        Treatment_Stop ();
	Comms_Bridge_Channel1_Msg (msg);
        UsartRpiSend (s_ans_ok);
    }
    
}


void Comms_Bridge_Channel1_Msg (char * msg_for_ch1)
{
    char buff [128];    
    
    // bridge the message
    sprintf(buff, "%s\r\n", msg_for_ch1);
    UsartChannel1Send (buff);
    
}


void Comms_Bridge_Rpi_With_Connectors (void)
{
    char local_buff [SIZEOF_LOCAL_BUFF];

    UsartRpiSend ("\r\n Bridge to Connectors Board -- bridge done to finish --\r\n");
    while (1)
    {
	if (UsartRpiHaveData())
	{
	    UsartRpiHaveDataReset();
	    UsartRpiReadBuffer(local_buff, SIZEOF_LOCAL_BUFF);

	    if (!strncmp(local_buff, "bridge done", sizeof("bridge done") - 1))
	    {
		UsartRpiSend ("\r\n Back to normal\r\n");
		return;
	    }
	    else
	    {
		// bridge the message
		Comms_Bridge_Conn_Msg (local_buff);
	    }
	}

	if (UsartConnHaveData())
	{
	    UsartConnHaveDataReset();
	    UsartConnReadBuffer(local_buff, SIZEOF_LOCAL_BUFF);

	    // bridge the message
	    char buff_snd [130];    
	    sprintf(buff_snd, "%s\r\n", local_buff);
	    UsartRpiSend (buff_snd);
	}
    }
}


void Comms_Bridge_Conn_Msg (char * local_buff)
{
    char buff_snd [130] = { 0 };
    // char other_buff [130];
    sprintf(buff_snd, "%s\r\n", local_buff);
    // unsigned char size = strlen (buff_snd);
    // sprintf(other_buff, "size: %d sent: %s", size, buff_snd);
    // UsartRpiSend(other_buff);
    UsartConnSend (buff_snd);
    Comms_Conn_Bridge_Once();
    

}
//---- End of File ----//
