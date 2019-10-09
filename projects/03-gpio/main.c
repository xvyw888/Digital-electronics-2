/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-09-27
 * Last update: 2019-07-16
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description: Blink a LED with delay function.
 * TODO: Verify functionality of LED blinker.
 * NOTE: Macro _BV converts a bit number into a byte value (see AVR Libc
 * Reference Manual).
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
#include <gpio.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_PIN     PB5
#define LED_PIN1    PB0
#define BLINK_DELAY 250
#define BUTTON      PD2
 
/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
  * Brief:  Main program. Blink a LED with delay function.
  * Input:  None
  * Return: None
  */
int main(void)
{
    /* Set output pin */

    /*DDRD &= ~_BV(BUTTON);*/
    GPIO_config_output(&DDRB, LED_PIN1);
    GPIO_config_output(&DDRB, LED_PIN);

    /* Turn LED off */

    /*PORTD |= _BV(BUTTON);*/ 
     GPIO_write(&PORTB, LED_PIN1, 0);
     GPIO_write(&PORTB, LED_PIN, 1);

    /* Infinite loop */
  
    for (;;)
    {
        /* Invert LED and delay */

      GPIO_toggle(&PORTB, LED_PIN1);
      GPIO_toggle(&PORTB, LED_PIN);
    
        _delay_ms(BLINK_DELAY);
      

    }

    return (0);
}

