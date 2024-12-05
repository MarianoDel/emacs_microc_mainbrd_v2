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


// Module Functions ------------------------------------------------------------
void Comms_Timeouts (void)
{
    if (comms_timeout)
        comms_timeout--;
}


unsigned char Comms_Rpi_Answering (void)
{
    if (comms_timeout)
        return 1;
    
    return 0;
}


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
    char buff [128];    
    
    // check if its own, broadcast or channel
    // for channel 1
    if (strncmp (msg_str, "ch1", sizeof("ch1") - 1) == 0)
    {
        // check enable or bridged
        if (strncmp ((msg_str + 4), "enable", sizeof("enable") - 1) == 0)
        {
            Ena_Ch1_On();
            UsartRpiSend(s_ans_ok);
        }
        else if (strncmp ((msg_str + 4), "disable", sizeof("disable") - 1) == 0)
        {
            Ena_Ch1_Off();
            UsartRpiSend(s_ans_ok);
        }
        else    // bridge the message
        {
            sprintf(buff, "%s\n", (msg_str + 4));
            UsartChannel1Send (buff);
        }
    }
    else if (strncmp (msg_str, "ch2", sizeof("ch2") - 1) == 0)
    {
        // check enable or bridged
        if (strncmp ((msg_str + 4), "enable", sizeof("enable") - 1) == 0)
        {
            Ena_Ch2_On();
            UsartRpiSend(s_ans_ok);
        }
        else if (strncmp ((msg_str + 4), "disable", sizeof("disable") - 1) == 0)
        {
            Ena_Ch2_Off();
            UsartRpiSend(s_ans_ok);
        }
        else    // check message for treatment info
        {
            Comms_Messages_For_Channels_Treatment (msg_str + 4);
        }
    }
    else if (strncmp (msg_str, "ch3", sizeof("ch3") - 1) == 0)
    {
        // check enable or bridged
        if (strncmp ((msg_str + 4), "enable", sizeof("enable") - 1) == 0)
        {
            Ena_Ch3_On();
            UsartRpiSend(s_ans_ok);
        }
        else if (strncmp ((msg_str + 4), "disable", sizeof("disable") - 1) == 0)
        {
            Ena_Ch3_Off();
            UsartRpiSend(s_ans_ok);
        }
        else    // check message for treatment info
        {
            Comms_Messages_For_Channels_Treatment (msg_str + 4);
        }
    }
    else if (strncmp (msg_str, "ch4", sizeof("ch4") - 1) == 0)
    {
        // check enable or bridged
        if (strncmp ((msg_str + 4), "enable", sizeof("enable") - 1) == 0)
        {
            Ena_Ch4_On();
            UsartRpiSend(s_ans_ok);
        }
        else if (strncmp ((msg_str + 4), "disable", sizeof("disable") - 1) == 0)
        {
            Ena_Ch4_Off();
            UsartRpiSend(s_ans_ok);
        }
        else    // check message for treatment info
        {
            Comms_Messages_For_Channels_Treatment (msg_str + 4);
        }
    }
    else if (strncmp (msg_str, "chf", sizeof("chf") - 1) == 0)
    {
        // check enable or bridged
        if (strncmp ((msg_str + 4), "enable", sizeof("enable") - 1) == 0)
        {
            Ena_Ch1_On();
            Ena_Ch2_On();
            Ena_Ch3_On();
            Ena_Ch4_On();            
            UsartRpiSend(s_ans_ok);
        }
        else if (strncmp ((msg_str + 4), "disable", sizeof("disable") - 1) == 0)
        {
            Ena_Ch1_Off();
            Ena_Ch2_Off();
            Ena_Ch3_Off();
            Ena_Ch4_Off();            
            UsartRpiSend(s_ans_ok);
        }
        else    // bridge the message
        {
            sprintf(buff, "%s\n", (msg_str + 4));
            UsartChannel1Send (buff);

	    // check message for treatment info
            Comms_Messages_For_Channels_Treatment (msg_str + 4);
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
    
    // else if (strncmp (msg_str, "rpi is up", sizeof("rpi is up") - 1) == 0)
    // {
    //     Bit_Bang_Tx_Send("rpi up\n");
    // }
    
    else if (strncmp (msg_str, "sup", sizeof("sup") - 1) == 0)
    {
        // not implemented yet!
        UsartRpiSend(s_ans_ok);
    }
    else
        UsartRpiSend(s_ans_nok);

}


void Comms_Messages_For_Channels_Treatment (char * msg_str)
{
    resp_e resp;
    
    char * msg = msg_str;
    
    // -- config messages for signals --
    if (!strncmp(msg, "frequency", sizeof("frequency") - 1))
    {
        resp = Treatment_SetFrequency_Str (msg + sizeof("frequency"));
        if (resp == resp_ok)
            UsartRpiSend (s_ans_ok);
        else
            UsartRpiSend (s_ans_nok);
    }

    else if (!strncmp(msg, "intensity", sizeof("intensity") - 1))
    {
        resp = Treatment_SetIntensity_Str (msg + sizeof("intensity"));
        if (resp == resp_ok)
            UsartRpiSend (s_ans_ok);
        else
            UsartRpiSend (s_ans_nok);
    }

    else if (!strncmp(msg, "polarity", sizeof("polarity") - 1))
    {
        resp = Treatment_SetPolarity_Str (msg + sizeof("polarity"));
        if (resp == resp_ok)
            UsartRpiSend (s_ans_ok);
        else
            UsartRpiSend (s_ans_nok);
    }

    // config messages for channel setup
    else if (!strncmp(msg, "mode", sizeof("mode") - 1))
    {
        resp = Treatment_SetMode_Str (msg + sizeof("mode"));
        if (resp == resp_ok)
            UsartRpiSend (s_ans_ok);
        else
            UsartRpiSend (s_ans_nok);
    }

    else if (!strncmp(msg, "threshold", sizeof("threshold") - 1))
    {
        resp = Treatment_SetThreshold_Str (msg + sizeof("threshold"));
        if (resp == resp_ok)
            UsartRpiSend (s_ans_ok);
        else
            UsartRpiSend (s_ans_nok);
    }

    // -- operation messages --
    else if (!strncmp(msg, "start", sizeof("start") - 1))
    {
        Treatment_Start ();
        UsartRpiSend (s_ans_ok);
    }

    else if (!strncmp(msg, "stop", sizeof("stop") - 1))
    {
        Treatment_Stop ();
        UsartRpiSend (s_ans_ok);
    }
    
}
//---- End of File ----//
