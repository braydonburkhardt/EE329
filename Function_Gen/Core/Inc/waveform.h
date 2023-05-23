#ifndef INC_WAVEFORM_H_
#define INC_WAVEFORM_H_

#include "stm32l4xx_hal.h"
#include "DAC.h"

//main clock freq (default 4Mhz)
#define _Sys_Freq 4000000

//sample frequency
#define waveform_sampleRate 20000

void waveform_init();
void TIM2_IRQHandler();

#endif
