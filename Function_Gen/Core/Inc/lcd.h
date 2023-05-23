#ifndef __LCD_H_
#define __LCD_H_

#include "stm32l4xx_hal.h"
#include "delay.h"

// define port and pins
#define LCD_PORT GPIOE
#define LCD_PORT_RCC RCC_AHB2ENR_GPIOEEN
#define LCD_PIN_RS 9
#define LCD_PIN_RW 8
#define LCD_PIN_EN 10
#define LCD_PIN_D7 7
#define LCD_PIN_D6 6
#define LCD_PIN_D5 5
#define LCD_PIN_D4 4

// commands
#define LCD_COMMAND_CLEAR 0x01
#define LCD_COMMAND_HOME 0x02
#define LCD_COMMAND_DISPLAY_ON 0x0C
#define LCD_COMMAND_DISPLAY_OFF 0x08
#define LCD_COMMAND_CURSOR_ON 0x0A
#define LCD_COMMAND_CURSOR_OFF 0x08
#define LCD_COMMAND_BLINK_ON 0x09
#define LCD_COMMAND_BLINK_OFF 0x08
#define LCD_COMMAND_ENTRY_INC 0x06
#define LCD_COMMAND_ENTRY_DEC 0x04
#define LCD_COMMAND_WRITE 0x80

void LCD_init();
void LCD_pulse();
void LCD_command(uint8_t);
void LCD_write(uint8_t);
void LCD_write_string(char input[2][16]);

#endif /* __LCD_H_ */
