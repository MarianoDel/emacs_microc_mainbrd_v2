//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.H #################################
//---------------------------------------------

#ifndef HARD_H_
#define HARD_H_


//----------- Defines For Configuration -------------

//----- Board Configuration -------------------//
//--- Hardware ------------------//
#define HARDWARE_VERSION_2_0    // first prototype of main board (second prot in project)


//--- Software ------------------//
#define FIRMWARE_VERSION_1_0    // init version


//-------- Type of Program (depending on software version) ----------------


//--- Serial Number / Device Id Bytes length ----------
#define USE_DEVICE_ID_4BYTES
// #define USE_DEVICE_ID_12BYTES


//-------- Type of Program and Features ----------------
#define RPI_CONN_ONLY_ONE    // rpi goes up with the first data
// #define RPI_CONN_BY_TIMER    // rpi goes up or down with timer



//-------- Oscillator and Crystal selection (Freq in startup_clocks.h) ---
#define HSI_INTERNAL_RC
// #define HSE_CRYSTAL_OSC

#ifdef HSE_CRYSTAL_OSC
// #define CRYSTAL_8MHZ
#define CRYSTAL_12MHZ
#endif

#ifdef HSE_CRYSTAL_OSC
// #define SYSCLK_FREQ_72MHz
#define SYSCLK_FREQ_8MHz
#endif

#ifdef HSI_INTERNAL_RC
#define SYSCLK_FREQ_64MHz
// #define SYSCLK_FREQ_8MHz
#endif

//-------- End Of Defines For Configuration ------




//--- Hardware & Software Messages ------------------//
#ifdef HARDWARE_VERSION_2_0
#define HARD "Hardware Version: 2.0"
#endif
#ifdef HARDWARE_VERSION_1_0
#define HARD "Hardware Version: 1.0"
#endif
#ifdef FIRMWARE_VERSION_1_0
#define SOFT "Firmware Version: 1.0"
#endif
//--- End of Hardware & Software Messages ------------------//



// Exported Types --------------------------------------------------------------
#ifdef HARDWARE_VERSION_2_0

// PA defines ----
// PA0 NC

// PA1
#define ENA_SPEAKER    ((GPIOA->ODR & 0x0002) != 0)
#define ENA_SPEAKER_ON    (GPIOA->BSRR = 0x00000002)
#define ENA_SPEAKER_OFF    (GPIOA->BSRR = 0x00020000)

// PA2 PA3 Alternative Usart2 Tx Rx
#define PA2    ((GPIOA->ODR & 0x0004) != 0)
#define PA2_ON    (GPIOA->BSRR = 0x00000004)
#define PA2_OFF    (GPIOA->BSRR = 0x00040000)
#define PA3   ((GPIOA->ODR & 0x0008) != 0)
#define PA3_ON    (GPIOA->BSRR = 0x00000008)
#define PA3_OFF    (GPIOA->BSRR = 0x00080000)

// PA4 Analog DAC Output1 (SIG_CH)
// PA5 Analog DAC Output2 (IN_PLUS_AUDIO)

// PA6 NC

// PA7 Alternative TIM8_CH1N (IN_RIGHT_CH2)
#define IN_RIGHT_CH2    ((GPIOA->ODR & 0x0080) != 0)
#define IN_RIGHT_CH2_ON    (GPIOA->BSRR = 0x00000080)
#define IN_RIGHT_CH2_OFF    (GPIOA->BSRR = 0x00800000)

// PA8 NC

// PA9 PA10 Alternative Usart1 Tx Rx

// PA11
#define ENA_RPI    ((GPIOA->ODR & 0x0800) != 0)
#define ENA_RPI_ON    (GPIOA->BSRR = 0x00000800)
#define ENA_RPI_OFF    (GPIOA->BSRR = 0x08000000)

// PA12
#define ENA_LCD    ((GPIOA->ODR & 0x1000) != 0)
#define ENA_LCD_ON    (GPIOA->BSRR = 0x00001000)
#define ENA_LCD_OFF    (GPIOA->BSRR = 0x10000000)

// PA13 PA14 PA15 NC jtag

// PB defines ----
// PB0 Alternative TIM8_CH2N (IN_RIGHT_CH3)
// PB1 Alternative TIM8_CH3N (IN_RIGHT_CH4)
#define IN_RIGHT_CH3    ((GPIOB->ODR & 0x0001) != 0)
#define IN_RIGHT_CH3_ON    (GPIOB->BSRR = 0x00000001)
#define IN_RIGHT_CH3_OFF    (GPIOB->BSRR = 0x00010000)

#define IN_RIGHT_CH4    ((GPIOB->ODR & 0x0002) != 0)
#define IN_RIGHT_CH4_ON    (GPIOB->BSRR = 0x00000002)
#define IN_RIGHT_CH4_OFF    (GPIOB->BSRR = 0x00020000)


//PB2
#define ENA_CH2    ((GPIOB->ODR & 0x0004) == 0)
#define ENA_CH2_OFF    (GPIOB->BSRR = 0x00000004)
#define ENA_CH2_ON    (GPIOB->BSRR = 0x00040000)

// PB3 PB4 NC jtag
// PB5
// PB6
// PB7
// PB8
// PB9 NC

// PB10 PB11 Alternative Usart3 Tx Rx

// PB12
#define ENA_CH3    ((GPIOB->ODR & 0x1000) == 0)
#define ENA_CH3_OFF    (GPIOB->BSRR = 0x00001000)
#define ENA_CH3_ON    (GPIOB->BSRR = 0x10000000)

// PB13
#define ENA_CH4    ((GPIOB->ODR & 0x2000) == 0)
#define ENA_CH4_OFF    (GPIOB->BSRR = 0x00002000)
#define ENA_CH4_ON    (GPIOB->BSRR = 0x20000000)

// PB14
#define ENA_ENC_CONN    ((GPIOB->ODR & 0x4000) == 0)
#define ENA_ENC_CONN_OFF    (GPIOB->BSRR = 0x00004000)
#define ENA_ENC_CONN_ON    (GPIOB->BSRR = 0x40000000)

// PB15 NC

// PC defines ----
// PC0
// PC1 NC

// PC2
#define SYNC_CH1    ((GPIOC->ODR & 0x0004) != 0)
#define SYNC_CH1_ON    (GPIOC->BSRR = 0x00000004)
#define SYNC_CH1_OFF    (GPIOC->BSRR = 0x00040000)

// PC3
#define ENA_CH1    ((GPIOC->ODR & 0x0008) == 0)
#define ENA_CH1_OFF    (GPIOC->BSRR = 0x00000008)
#define ENA_CH1_ON    (GPIOC->BSRR = 0x00080000)

// PC4
// PC5 NC

// PC6 Alternative TIM8_CH1 (IN_LEFT_CH2)
// PC7 Alternative TIM8_CH2 (IN_LEFT_CH3)
// PC8 Alternative TIM8_CH3 (IN_LEFT_CH4)

// PC6 PC7 PC8 as push-pull
#define IN_LEFT_CH2    ((GPIOC->ODR & 0x0040) != 0)
#define IN_LEFT_CH2_ON    (GPIOC->BSRR = 0x00000040)
#define IN_LEFT_CH2_OFF    (GPIOC->BSRR = 0x00400000)

#define IN_LEFT_CH3    ((GPIOC->ODR & 0x0080) != 0)
#define IN_LEFT_CH3_ON    (GPIOC->BSRR = 0x00000080)
#define IN_LEFT_CH3_OFF    (GPIOC->BSRR = 0x00800000)

#define IN_LEFT_CH4    ((GPIOC->ODR & 0x0100) != 0)
#define IN_LEFT_CH4_ON    (GPIOC->BSRR = 0x00000100)
#define IN_LEFT_CH4_OFF    (GPIOC->BSRR = 0x01000000)

// PC9 NC

// PC10 PC11 Alternative Uart4 Tx Rx

// PC12 Alternative Uart5 Tx

// PC13 PC14 PC15 NC

// PD defines ----
// PD0 PD1 NC

// PD2 Alternative Uart5 Rx

#endif //HARDWARE_VERSION_2_0



//--- Exported Module Functions ----
void Hard_GetVoltages (char * buff);
void Hard_GetHardSoft (char * buff);
void Hard_GetVoltages_Complete (void);

void Ena_Rpi_On (void);
void Ena_Rpi_Off (void);

void Ena_Lcd_On (void);
void Ena_Lcd_Off (void);

void Ena_Enc_Conn_On (void);
void Ena_Enc_Conn_Off (void);

void Ena_Ch1_On (void);
void Ena_Ch2_On (void);
void Ena_Ch3_On (void);
void Ena_Ch4_On (void);

void Ena_Ch1_Off (void);
void Ena_Ch2_Off (void);
void Ena_Ch3_Off (void);
void Ena_Ch4_Off (void);

unsigned char Ena_Ch1_Is_On (void);
unsigned char Ena_Ch2_Is_On (void);
unsigned char Ena_Ch3_Is_On (void);
unsigned char Ena_Ch4_Is_On (void);

void Left_Off (void);
void Left_On (void);
void Right_Off (void);
void Right_On (void);

unsigned char Left_Is_On (void);

void Sync_Ch1_Off (void);
void Sync_Ch1_On (void);


void Speaker_On (void);
void Speaker_Off (void);

#endif
