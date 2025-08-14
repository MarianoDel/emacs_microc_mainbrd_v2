//---------------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS_ENCODERS.C ##################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comms_encoders.h"
#include "usart_channels.h"
#include "usart.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Module Private Types Constants and Macros -----------------------------------
#define SIZEOF_ENCODERS_BUFF    128
// #define COMMS_TT_RELOAD    3000


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
char encod_buff [SIZEOF_ENCODERS_BUFF];


// Module Private Functions ----------------------------------------------------
static void Comms_Encoders_Messages (char * msg_str);



// Module Functions ------------------------------------------------------------
void Comms_Encoders_Update (void)
{
    if (UsartEncHaveData())
    {
        UsartEncHaveDataReset();
        UsartEncReadBuffer(encod_buff, SIZEOF_ENCODERS_BUFF);
        Comms_Encoders_Messages(encod_buff);
    }
}


static void Comms_Encoders_Messages (char * msg_str)
{
    char buff [128];
    sprintf(buff, "%s\r\n", msg_str);	    
    UsartRpiSend(buff);
}


//---- End of File ----//
