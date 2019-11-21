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
#include <avr/io.h>
#include "gpio.h"

/* Typedef -----------------------------------------------------------*/

/* Define ------------------------------------------------------------*/

/* Variables ---------------------------------------------------------*/

/* Function prototypes -----------------------------------------------*/

void fsm_random(void);
extern uint8_t generator_asm(uint8_t current);

int main (void)                                     //telo funkce, ktera nic nevraci 
{
    GPIO_config_output(&DDRB , PB5);
    
    for (;;) 
    {
        generator_asm(3) ; 
    }
    return (0);
}
