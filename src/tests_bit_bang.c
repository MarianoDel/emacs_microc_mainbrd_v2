//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "bit_bang.h"

// helper modules
#include "tests_ok.h"
// #include "tests_mock_usart.h"

#include <stdio.h>
#include <string.h>


// Types Constants and Macros --------------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Functions to Test ----------------------------------------------------
void Test_Bit_Bang_Send (void);
void Test_Bit_Bang_Tx_Handler (void);


// Module Auxiliary Functions --------------------------------------------------



// Tests Module Auxiliary or General Functions ---------------------------------


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{

    Test_Bit_Bang_Send ();

    Test_Bit_Bang_Tx_Handler ();

    Test_Bit_Bang_Rx_Handler ();

    return 0;
}


extern volatile unsigned char tx_bytes_left;
extern volatile unsigned char tx_buff[];
void Test_Bit_Bang_Send (void)
{
    int error = 0;
    char my_buff [100];

    strcpy(my_buff, "Mariano");
    int my_len = strlen(my_buff);

    Bit_Bang_Tx_Send(my_buff);

    printf("Test Send %d bytes left: ", tx_bytes_left);    
    if (tx_bytes_left == my_len)
        PrintOK();
    else
        PrintERR();

    if (strncmp(my_buff, tx_buff, my_len) != 0)
        error = 1;
    
    printf("Test Send %s bytes buff: ", tx_buff);    
    if (error)
        PrintERR();
    else
        PrintOK();        

}

int tx_ints = 0;
int tx_loop = 0;
void Test_Bit_Bang_Tx_Handler (void)
{
    int error = 0;
    char my_buff [100];

    strcpy(my_buff, "MMM");
    int my_len = strlen(my_buff);

    Bit_Bang_Init();
    Bit_Bang_Tx_Send(my_buff);

    if (tx_ints)
    {
        while (tx_loop)
        {
            Bit_Bang_Tx_Handler();
        }
    }

    printf ("ints: %d tx_bytes_left: %d tx_loop: %d\n",
            tx_ints,
            tx_bytes_left,
            tx_loop);
        
}



extern volatile unsigned char rx_buff[];
extern volatile unsigned char rx_ready;
extern volatile unsigned char rx_bytes_cnt;
unsigned char rx_pin_value = 0;
int int_by_driver = 0;
void Test_Bit_Bang_Rx_Handler (void)
{
    int error = 0;
    char my_buff [100];

    // strcpy(my_buff, "MMMM\n");
    strcpy(my_buff, "Mariano\nMariano\n");    
    int my_len = strlen(my_buff);

    Bit_Bang_Init();

    printf ("\n\nTesting Rx\n");
    // start
    int my_bytes_cnt = 0;
    int byte_to_send = 0;
    int my_mask = 0;

    // force first rx init
    Bit_Bang_Rx_Int_Handler ();        
    
    do {

        byte_to_send = my_buff[my_bytes_cnt];
        printf (" sending %x\n", byte_to_send);
        my_mask = 0x80;
        
        // send start
        if (int_by_driver)
        {
            int_by_driver = 0;
            Bit_Bang_Rx_Int_Handler ();
        }
        
        rx_pin_value = 0;
        printf(" %dS", rx_pin_value);
        Bit_Bang_Rx_Tim_Handler();
        if (rx_ready)
            printf ("data is ready! on start\n");
        
        for (int i = 0; i < 16; i++)
        {
            if (byte_to_send & my_mask)
                rx_pin_value = 1;
            else
                rx_pin_value = 0;

            if (i & 0x01)    // transition
            {
                my_mask >>= 1;
            }

            printf(" %d", rx_pin_value);
            Bit_Bang_Rx_Tim_Handler();

            if (rx_ready)
                printf ("data is ready! on bytes\n");
        }        

        // send stop
        rx_pin_value = 1;
        printf(" %d", rx_pin_value);
        Bit_Bang_Rx_Tim_Handler();
        rx_pin_value = 1;
        printf(" %dP\n", rx_pin_value);
        Bit_Bang_Rx_Tim_Handler();

        my_len--;
        my_bytes_cnt++;
        
        printf ("  bytes_send: %d rx_buff[%d]: %x\n",
                my_bytes_cnt,
                rx_bytes_cnt - 1,
                rx_buff[rx_bytes_cnt - 1]);

        if (Bit_Bang_Rx_Have_Data())
        {
            int len = 0;
            char my_rx [100];
            len = Bit_Bang_Rx_ReadBuffer(&my_rx);
            printf("data len: %d rx: %s\n", len, my_rx);
        }
        
        // if (rx_ready)
        // {
        //     printf ("data is ready! on stop\n");
        //     }
        // }
        
    } while (my_len);
        
}


// void Test_Comms_With_Usart (void)
// {
//     // set callback on usart1 RpiSend()
//     Usart1Callback(CB_Usart);


//     cb_usart_value = 0;
//     Usart2FillRxBuffer("loco\r\n");    
//     Comms_Channel1 ();

//     printf("Test error string on channel 1: ");
//     if (cb_usart_value == 0)    //no answer
//         PrintOK();
//     else
//         PrintERR();


//     // cb_usart_value = 0;    
//     // Usart2FillRxBuffer("name:Tunel\r\n");
//     // Comms_Channel1 ();
    
//     // printf("Test antenna name channel 1: ");
//     // if (cb_usart_value == 2)
//     //     PrintOK();
//     // else
//     //     PrintERR();


//     char my_ant_str [] = { "ant3,003.50,019.00,003.50,065.00\r\n" };
//     cb_usart_value = 0;    
//     Usart2FillRxBuffer(my_ant_str);
//     Comms_Channel1 ();
//     global_ant.resistance_int = 0;
//     global_ant.resistance_dec = 0;
//     global_ant.inductance_int = 0;
//     global_ant.inductance_dec = 0;

    
//     printf("Test old antenna & parse: ");
//     if ((cb_usart_value == 4) &&
//         (global_ant.resistance_int == 0) &&
//         (global_ant.resistance_dec == 0) &&
//         (global_ant.inductance_int == 0) &&
//         (global_ant.inductance_dec == 0))
//     {
//         PrintOK();
//     }
//     else
//         PrintERR();

//     cb_usart_value = 0;    
//     Usart2FillRxBuffer("temp,055.55\r\n");
//     Comms_Channel1 ();
    
//     printf("Test antenna current temp: ");
//     if (cb_usart_value == 2)
//         PrintOK();
//     else
//         PrintERR();

//     cb_usart_value = 0;
//     antenna_answered = 0;    
//     Usart2FillRxBuffer("ok\r\n");
//     Comms_Channel1 ();
    
//     printf("Test antenna keepalive answer: ");
//     if ((cb_usart_value == 0) && (antenna_answered))
//         PrintOK();
//     else
//         PrintERR();

//     char my_ant_str2 [] = { "ant3,0aa.50,019.00,003.50,065.50\r\n" };    
//     cb_usart_value = 0;    
//     Usart2FillRxBuffer(my_ant_str2);
//     Comms_Channel1 ();
    
//     global_ant.resistance_int = 0;
//     global_ant.resistance_dec = 0;
//     global_ant.inductance_int = 0;
//     global_ant.inductance_dec = 0;
    
//     printf("Test new antenna with errors on params: ");
//     if ((cb_usart_value == 3) &&
//         (global_ant.resistance_int == 0) &&
//         (global_ant.resistance_dec == 0) &&
//         (global_ant.inductance_int == 0) &&
//         (global_ant.inductance_dec == 0))
//         PrintOK();
//     else
//         PrintERR();

//     cb_usart_value = 0;    
//     Usart2FillRxBuffer("temp,0aa.55\r\n");
//     Comms_Channel1 ();
    
//     printf("Test antenna error on temp integer: ");
//     if (cb_usart_value == 0)
//         PrintOK();
//     else
//         PrintERR();

//     cb_usart_value = 0;    
//     Usart2FillRxBuffer("temp,055.aa\r\n");
//     Comms_Channel1 ();
    
//     printf("Test antenna error on temp decimal: ");
//     if (cb_usart_value == 0)
//         PrintOK();
//     else
//         PrintERR();

//     global_ant.resistance_int = 0;
//     global_ant.resistance_dec = 0;
//     global_ant.inductance_int = 0;
//     global_ant.inductance_dec = 0;
        
//     strcpy (my_ant_str2,"ant3,003.50,019.00,003.50,065.50\r\n");
//     cb_usart_value = 0;    
//     Usart2FillRxBuffer(my_ant_str2);
//     Comms_Channel1 ();
    
//     printf("Test new antenna with good params: ");
//     if ((cb_usart_value == 3) &&
//         (global_ant.resistance_int == 3) &&
//         (global_ant.resistance_dec == 50) &&
//         (global_ant.inductance_int == 19) &&
//         (global_ant.inductance_dec == 0) &&
//         (global_ant.current_limit_int == 3) &&
//         (global_ant.current_limit_dec == 50) &&
//         (global_ant.temp_max_int == 65) &&
//         (global_ant.temp_max_dec == 50))
//         PrintOK();
//     else
//         PrintERR();
    
// }


// Module Mocked Functions -----------------------------------------------------
void Tim_Bit_Bang_Tx_Reset(void)
{
    printf("tim int reset\n");
    tx_loop = 0;
}


void Tim_Bit_Bang_Tx_Set(void)
{
    printf("tim int set\n");
    tx_ints++;
    tx_loop = 1;
}


void Tx_Pin_On (void)
{
    printf(" 1");
}


void Tx_Pin_Off (void)
{
    printf(" 0");
}

unsigned char Rx_Pin (void)
{
    return rx_pin_value;
}

void Rx_Int_Enable (void)
{
    printf("\nrx int enabled by driver\n");
    int_by_driver = 1;
}


void Rx_Int_Disable (void)
{
    printf("\nrx int disabled by driver\n");
}

void Tim_Bit_Bang_Rx_Set (void)
{
    printf("\ntim enabled by driver\n");
}

void Tim_Bit_Bang_Rx_Reset (void)
{
    printf("\ntim disabled by driver\n");
}
//--- end of file ---//


