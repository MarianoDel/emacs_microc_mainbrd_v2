//---------------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS_CHANNEL1.C ######################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comms_channel1.h"
#include "hard.h"

#include "usart_channels.h"
#include "usart.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Module Private Types Constants and Macros -----------------------------------
#define SIZEOF_CHANNEL1_BUFF    128
// #define COMMS_TT_RELOAD    3000


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
char channel1_buff [SIZEOF_CHANNEL1_BUFF];
// volatile unsigned short comms_timeout = 0;


// Module Private Functions ----------------------------------------------------
static void Comms_Channel1_Messages (char * msg_str);
void Comms_Bridge_Rpi_Msg (char * msg_for_rpi);


// Module Functions ------------------------------------------------------------
void Comms_Channel1_Update (void)
{
    if (UsartChannel1HaveData())
    {
        UsartChannel1HaveDataReset();
        UsartChannel1ReadBuffer(channel1_buff, SIZEOF_CHANNEL1_BUFF);
        Comms_Channel1_Messages(channel1_buff);
    }
}


static void Comms_Channel1_Messages (char * msg_str)
{
    Comms_Bridge_Rpi_Msg (msg_str);
    if (!strncmp(msg_str, "probe start", (sizeof("probe start") - 1)))
    {
	// for (int i = 0; i < 10; i++)
	// {
	//     Sync_Ch1_On();
	//     Wait_ms(250);
	//     Sync_Ch1_Off();
	//     Wait_ms(250);
	// }
    }
}


void Comms_Bridge_Rpi_Msg (char * msg_for_rpi)
{
    char buff [128];    
    
    // bridge the message
    sprintf(buff, "%s\r\n", msg_for_rpi);
    UsartRpiSend (buff);
    
}


//---- End of File ----//
