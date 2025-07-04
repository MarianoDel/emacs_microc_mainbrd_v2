//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### COMMS_CHANNEL1.H ##########################
//---------------------------------------------

#ifndef _COMMS_CHANNEL1_H_
#define _COMMS_CHANNEL1_H_

//---- Includes to help the Defines ----------



//---- Configurations Defines --------------------


// Module Exported Functions ---------------------------------------------------
void Comms_Channel1_Update (void);
void Comms_Channel1_Send_Default_Set (void);
void Comms_Channel1_Send_Default_Reset (void);
unsigned char Comms_Channel1_Send_Default_Get (void);


#endif    /* _COMMS_CHANNEL1_H_ */
