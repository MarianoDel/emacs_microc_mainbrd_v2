//---------------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS_CONN.C ######################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comms_conn.h"
#include "hard.h"

#include "usart_channels.h"
#include "usart.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Module Private Types Constants and Macros -----------------------------------
#define SIZEOF_CONN_BUFF    128
// #define COMMS_TT_RELOAD    3000


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
char conn_buff [SIZEOF_CONN_BUFF];
// volatile unsigned short comms_timeout = 0;
unsigned char last_plates = 0;
unsigned char last_intel_prb = 0;


// Module Private Functions ----------------------------------------------------
static void Comms_Conn_Messages (char * msg_str);
static unsigned char Comms_Conn_Hex2Int (char ch);


// Module Functions ------------------------------------------------------------
void Comms_Conn_Update (void)
{
    if (UsartConnHaveData())
    {
        UsartConnHaveDataReset();
        UsartConnReadBuffer(conn_buff, SIZEOF_CONN_BUFF);
        Comms_Conn_Messages(conn_buff);
    }
}


static void Comms_Conn_Messages (char * msg_str)
{
    unsigned char plates_up;
    unsigned char plates_dw;
    unsigned char plates;
    unsigned char intel_prb;    
    
    // string getted: "conn 0x%02x 0x%02x\r\n"
    // check if enable is needed, broadcast or channel
    if (strncmp (msg_str, "conn 0x", sizeof("conn 0x") - 1) == 0)
    {
	// get the conn data
	plates_up = Comms_Conn_Hex2Int (*(msg_str + sizeof("conn 0x") - 1));
	plates_dw = Comms_Conn_Hex2Int (*(msg_str + sizeof("conn 0xx") - 1));
	intel_prb = Comms_Conn_Hex2Int (*(msg_str + sizeof("conn 0xxx 0xx") - 1));

	plates = plates_up;
	plates <<= 4;
	plates |= plates_dw;

	if ((plates != last_plates) ||
	    (intel_prb != last_intel_prb))
	{
	    char buff [128];

	    last_plates = plates;
	    last_intel_prb = intel_prb;
	    
	    // sprintf(buff, "%s new plates: 0x%02x probe: 0x%02x\r\n", msg_str, plates, intel_prb);
	    sprintf(buff, "%s\r\n", msg_str);	    
	    UsartRpiSend(buff);

	    // check enable or disable
	    if ((plates & 0x03) == 0x03)
		Ena_Ch4_On();
	    else
		Ena_Ch4_Off();

	    if ((plates & 0x0C) == 0x0C)
		Ena_Ch3_On();
	    else
		Ena_Ch3_Off();

	    if ((plates & 0x30) == 0x30)
		Ena_Ch2_On();
	    else
		Ena_Ch2_Off();		

	    if (((plates & 0xC0) == 0xC0) ||
		((plates & 0x40) && (intel_prb)))
		Ena_Ch1_On();
	    else
		Ena_Ch1_Off();
	    
	}
    }
}


static unsigned char Comms_Conn_Hex2Int (char ch)
{
    unsigned char a = 0;
    
    if ((ch >= '0') && (ch <= '9'))
       a = ch - '0';
    if ((ch >= 'A') && (ch <= 'F'))
       a = ch - 'A' + 10;
    if ((ch >= 'a') && (ch <= 'f'))
       a = ch - 'a' + 10;

    return a;
}

//---- End of File ----//
