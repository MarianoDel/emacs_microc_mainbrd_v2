//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### AUDIO.H ################################
//------------------------------------------------
#ifndef _AUDIO_H_
#define _AUDIO_H_


// Module Exported Types Constants and Macros ----------------------------------
typedef enum {
    AUDIO_RESET_EVENT,
    AUDIO_START_UP_EVENT,
    AUDIO_START_DWN_EVENT,    
    AUDIO_TONE_END_EVENT    

} audio_sm_event_e;

typedef enum {
    AUDIO_VOLUME_SET_FULL,
    AUDIO_VOLUME_SET_HIGH,
    AUDIO_VOLUME_SET_HALF,
    AUDIO_VOLUME_SET_LOW,
    AUDIO_VOLUME_SET_OFF
    
} audio_volume_set_e;


// Exported Module Functions ---------------------------------------------------
void Audio_Init (void);
void Audio_Start_Tone (unsigned short time_ms, unsigned short freq_hz);
void Audio_Timer_Set_Params (unsigned short new_arr);
void Audio_Timer_Interrupt_Handler (void);
void Audio_SM (audio_sm_event_e new_event);
void Audio_Volume_Set (audio_volume_set_e new_volume);


#endif    /* _AUDIO_H_ */

//--- end of file ---//

