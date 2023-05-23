#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "stm32l4xx_hal.h"

//specify row and col ports here (can be the same)
#define ROW_PORT GPIOB
#define ROW_CLK_PORT RCC_AHB2ENR_GPIOBEN
#define COL_PORT GPIOD
#define COL_CLK_PORT RCC_AHB2ENR_GPIODEN

//specify gpio pins here as uints
#define ROW_1_PIN 0
#define ROW_2_PIN 1
#define ROW_3_PIN 2
#define ROW_4_PIN 3

#define COL_1_PIN 0
#define COL_2_PIN 1
#define COL_3_PIN 2
#define COL_4_PIN 3

void keypad_init();
unsigned int keypad_rowDetect();
unsigned int keypad_colDetect();
int keypad_getPressedKey();

#endif
