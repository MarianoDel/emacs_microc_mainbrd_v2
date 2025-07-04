//---------------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS_SUPPLY.C ######################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comms_supply.h"
#include "hard.h"

#include "usart_channels.h"
#include "usart.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Module Private Types Constants and Macros -----------------------------------
#define SIZEOF_SUPPLY_BUFF    128
// #define COMMS_TT_RELOAD    3000


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
char supply_buff [SIZEOF_SUPPLY_BUFF];


// Module Private Functions ----------------------------------------------------
static void Comms_Supply_Messages (char * msg_str);


// Module Functions ------------------------------------------------------------
void Comms_Supply_Update (void)
{
    if (UsartSupplyHaveData())
    {
        UsartSupplyHaveDataReset();
        UsartSupplyReadBuffer(supply_buff, SIZEOF_SUPPLY_BUFF);
        Comms_Supply_Messages(supply_buff);
    }
}


static void Comms_Supply_Messages (char * msg_str)
{
    if (strncmp (msg_str, "supply", sizeof("supply") - 1) == 0)
    {
	char buff [128];    
    
	// bridge the message
	sprintf(buff, "%s\r\n", msg_str);
	UsartRpiSend (buff);
    }
}


//---- End of File ----//
