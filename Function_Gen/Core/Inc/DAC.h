#ifndef INC_DAC_H_
#define INC_DAC_H_

#include "stm32l4xx_hal.h"

//define max allowable Vout in mV
#define max_voltage 3300

//calibration parameters in mV
#define DAC_cal_multiplier 1
#define DAC_cal_offset 12

//define gpio pins used for SPI1
#define DAC_PORT GPIOA
#define DAC_PORT_RCC RCC_AHB2ENR_GPIOAEN
#define DAC_CS 4
#define DAC_SCK 5
#define DAC_COPI 7

void DAC_init();
unsigned int DAC_volt_conv(unsigned int voltage);
void DAC_write(unsigned int spiData);

#endif
