/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
                Tomas Lorenc
                Josef Holub
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-11
 * Last update: 2019-10-16
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
* Description:
 *    Decimal counter with data output on LCD display.
 * 
 * Note:
 *    Modified version of Peter Fleury's LCD library with R/W pin 
 *    connected to GND. Newline symbol "\n" is not implemented, use
 *    lcd_gotoxy() function instead.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>             // itoa() function
#include "timer.h"
#include "lcd.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Shows decimal values ​​on LCD display.
 *  Input:  None
 *  Return: None
 */

    uint8_t  value = 0;
    char  lcd_string [8];

     uint8_t  lcd_user_symbols [] = 
    { 0x0E,
    0x11,
    0x1B,
    0x11,
    0x11,
    0x1F,
    0x11,
    0x0E, 0x0E,
  0x11,
  0x1B,
  0x11,
  0x11,
  0x15,
  0x11,
  0x0E};

int main(void)
{
    /* LCD display
     * TODO: See Peter Fleury's online manual for LCD library 
     * http://homepage.hispeed.ch/peterfleury/avr-software.html
     * Initialize display and test different types of cursor */
    lcd_init(LCD_DISP_ON);

    // Display string without auto linefeed
    lcd_gotoxy(0,0);
    lcd_puts("Counter: ");
    lcd_gotoxy(0,1);
    lcd_putc('$');
    lcd_gotoxy(6,1);
    lcd_puts("0b");

    // TODO: Display variable value in decimal, binary, and hexadecimal

    //Timer1
    TIM_config_prescaler(TIM1, TIM_PRESC_64);              //rychlost čítání
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    /* TODO: Design at least two user characters and store them in 
     *       the display memory */
     lcd_command(1<<LCD_CGRAM);                             //zobrazení znaku
     for (uint8_t i = 0; i < sizeof(lcd_user_symbols); i++){
         lcd_data(lcd_user_symbols[i]);
     }
     
    lcd_gotoxy(14,0);               //vlastni symbol
    lcd_putc(0x00);
    lcd_putc(0x01);

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    for (;;) {
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Increment counter value.
 */
ISR(TIMER1_OVF_vect)                            // vnoření po vyvolaní přerušení cca. kazdych 262ms
{
    // TODO: Increment counter value form 0 to 255
    value++;

    itoa(value , lcd_string , 16);
    lcd_gotoxy(1,1);
    lcd_puts("   ");
    lcd_gotoxy(1,1);
    lcd_puts(lcd_string);

    itoa(value , lcd_string , 10);
    lcd_gotoxy(9,0);
    lcd_puts("   ");
    lcd_gotoxy(9,0);
    lcd_puts(lcd_string);

    itoa(value , lcd_string , 2);
    lcd_gotoxy(8,1);
    lcd_puts("        ");
    lcd_gotoxy(8,1);
    lcd_puts(lcd_string);
}
