/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2017-10-27
 * Last update: 2019-11-08
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Scan TWI bus for all connected slave devices and transmit info
 *    to UART.
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "uart.h"
#include "twi.h"

/* Typedef -----------------------------------------------------------*/
typedef enum {
    IDLE_STATE = 1,
    TRY_STATE,
    ACK_STATE
} state_t;

/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600

/* Variables ---------------------------------------------------------*/
state_t current_state = IDLE_STATE;

/* Function prototypes -----------------------------------------------*/
void temp(void);

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Test all TWI slave addresses and scan 
 *          connected devices.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    // UART: asynchronous, 8-bit data, no parity, 1-bit stop
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    // TWI
    twi_init();

    /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_8);              //rychlost čítání
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put strings to ringbuffer for transmitting via UART.
    //uart_puts("\r\n---TWI scanner---");
//    uart_puts("\r\n     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");

    // Infinite loop
    for (;;) {
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Update state of TWI Finite
 *         State Machine.
 */
ISR(TIMER1_OVF_vect)                                //volani funkce
{
    temp();
}

/**
 *  Brief: Test one TWI address.
 */
void temp(void)                                     //telo funkce, ktera nic nevraci 
{
    uint8_t temp_cel;                               //deklerace promennych
    uint8_t temp_des;
    char uart_string[3];                            

if (!twi_start((0x5c<<1) +TWI_WRITE))               //fungovalo by to i bez negace a if, ale takhle mame pojistene nechtene spusteni
    {                                               //podle stuktury komunikace TWI zavolame fci START a do registru pro urcite cidlo, zapiseme bit pro cteni
        twi_write(0x02);                            //chceme pouze cist teplotu, zjisteno v Datasheet
        twi_stop();
   
    if (!twi_start((0x5c<<1)+TWI_READ))             //zde jdeme cist data z cidla
        {
            temp_cel= twi_read_ack();               // twi read ack je cislo MSB dulezitejsi, vetsinou to co neni za des. tec.
            temp_des= twi_read_nack();              //cislo za des. teckou
            twi_stop();                             //ukonceni komunikace podle protokolu TWI
            itoa(temp_cel,uart_string,10);
            uart_puts("                     \r");      // formalni zapis cisla a UART zobrazeni
            uart_puts(uart_string);
            uart_puts(",");
            itoa(temp_des,uart_string,10);          //vyuziti fce itoa pro prevod do citelnejsi formy pro AVRko tz. on nevi co je hodnota 25,6 z cidla, my mu to musime pres itoa dat touhle formou
            uart_puts(uart_string);
            uart_puts("°C");
        }
    }

}
