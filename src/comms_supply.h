//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### COMMS_SUPPLY.H ##########################
//---------------------------------------------

#ifndef _COMMS_SUPPLY_H_
#define _COMMS_SUPPLY_H_

//---- Includes to help the Defines ----------



//---- Configurations Defines --------------------


// Module Exported Functions ---------------------------------------------------
void Comms_Supply_Update (void);
void Comms_Supply_Shutdown_Set (unsigned char new_shut_val);
void Comms_Supply_Shutdown_Reset (void);
unsigned char Comms_Supply_Shutdown_Get (void);



#endif    /* _COMMS_SUPPLY_H_ */
