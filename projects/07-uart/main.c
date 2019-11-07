/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-23
 * Last update: 2019-11-01
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Analog-to-digital conversion with displaying result on LCD and 
 *    transmitting via UART.
 * 
 * Note:
 *    Peter Fleury's UART library.
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>             // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "lcd.h"
#include "uart.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Read ADC result and transmit via UART.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    // LCD display
    lcd_init(LCD_DISP_ON);

    // UART: asynchronous, 8-bit data, no parity, 1-bit stop
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Display string without auto linefeed
    lcd_puts("LCD testing     ");
    // Put string to ringbuffer for transmitting via UART.
    uart_puts("UART testing\r\n");

    /* ADC
     * TODO: Configure ADC reference, clock source, enable ADC module, 
     *       and enable conversion complete interrupt */

    ADMUX |=_BV(REFS0);
    ADCSRA |=_BV(ADPS2) | _BV(ADPS1)| _BV(ADPS0);
    ADCSRA|=_BV(ADEN);
    ADCSRA|=_BV(ADIE);

    /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */

    TIM_config_prescaler(TIM1, TIM_PRESC_64);              //rychlost čítání
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    // Infinite loop
    for (;;) 
    {
        uint8_t c;

        c=uart_getc();
        if (c !='\0') 
        {        
            if (c==0x0d) 
            {
                lcd_gotoxy(9,1);
                lcd_clrscr();
            }   
            else 
            {
                lcd_putc(c);    
            }
        }
     }
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Start ADC conversion.
 */
ISR(TIMER1_OVF_vect)
{
    ADCSRA|=_BV(ADSC);   
}

/**
 *  Brief: ADC complete interrupt routine. Update LCD and UART 
 *         transmiter.
 */
ISR(ADC_vect)
{
    uint16_t value = 0;
    char uart_string[4];

	// Read 10-bit ACD value
     value = ADC;

    // TODO: Update LCD and UART transmiter
    
    itoa(value, uart_string, 10);
    //lcd_puts (uart_string);
    //uart_puts(uart_string);
    
    

    if (value <= 50)
    {
        uart_puts ("\033[4;36m");
        uart_puts("Right\n\r");
        uart_puts ("\033[0m");
        lcd_puts("Right");
        lcd_gotoxy(0,1);
    }
    else if (value <= 150)
    {
        uart_puts("Up\n\r");
        lcd_puts("Up");
        lcd_gotoxy(0,1);
    }
     else if (value <= 250)
    {
        uart_puts("Down\n\r");
        lcd_puts("Down");
        lcd_gotoxy(0,1);
    }

    else if (value <= 450)
    {
        uart_puts("Left\n\r");
        lcd_puts("Left");
        lcd_gotoxy(0,1);
    }
    else if (value <= 700)
    {
        uart_puts("Select\n\r");
        lcd_puts("Select");
        lcd_gotoxy(0,1);
    }

}

