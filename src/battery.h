//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### BATTERY.H ################################
//------------------------------------------------
#ifndef _BATTERY_H_
#define _BATTERY_H_



// Module Exported Types Constants and Macros ----------------------------------
// inputs divider/multiplier 0.211
#define MAINS_VOLTAGE_MIN    2625    // 10V
#define BATTERY_DISCONNECT_VOLTAGE    1701    // 6.5V
#define BATTERY_CONNECT_VOLTAGE    1832    // 7V

#define BATT_ADC_0    1571    // 6.0V
#define BATT_ADC_1    1780    // 6.8V
#define BATT_ADC_2    1911    // 7.3V
#define BATT_ADC_3    2042    // 7.8V
#define BATT_ADC_4    2147    // 8.2V

// for tests only
// #define BATTERY_DISCONNECT_VOLTAGE    1964    // 7.5V
// #define BATTERY_CONNECT_VOLTAGE    2094    // 8V

typedef enum {
    STATUS_UNKNOWN,
    RUNNING_ON_MAINS,
    RUNNING_ON_BATTERY
    
} battery_status_e;


// Exported Module Functions ---------------------------------------------------
void Battery_Status (void);
void Battery_Timeout (void);
battery_status_e Battery_Check (void);
unsigned short Battery_Get_Mains (void);
unsigned short Battery_Get_BatA (void);
unsigned short Battery_Get_BatB (void);
void Battery_Convert_To_Volts (unsigned short adc_value,
                               unsigned char * v_int,
                               unsigned char * v_dec);
char Battery_Convert_Status_From_Adc (unsigned short adc_batt);

#endif    /* _BATTERY_H_ */

//--- end of file ---//

