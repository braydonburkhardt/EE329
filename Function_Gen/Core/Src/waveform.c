#include "waveform.h"

int waveform_clk_freq = 100; //Hz
int waveform_data_buffer = 0; //mV

int waveformFlag_newPeriod = 0; //high when ready for new period
int waveformFlag_newSample = 0; //high when ready for next sample

void waveform_init(){
	DAC_init();

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; //TIM2 clk enable
	TIM2->DIER |= (TIM_DIER_CC1IE | TIM_DIER_UIE); //event gen enable
	TIM2->ARR = _Sys_Freq/waveform_clk_freq + 1;
	//TIM2->ARR = 0xFFFFFFFF;
	TIM2->CCR1 = _Sys_Freq/waveform_sampleRate + 1;
	TIM2->SR &= ~(TIM_SR_CC1IF | TIM_SR_UIF); //clear irq flag
	NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1F)); //set NVICint
	__enable_irq();
	TIM2->CR1 |= TIM_CR1_CEN; //begin timer
}

void TIM2_IRQHandler(void) {
	//CCR flag -> write next sample
    if (TIM2->SR & TIM_SR_CC1IF) {
      TIM2->SR &= ~(TIM_SR_CC1IF); //clear flag
      SPI1->DR = 0x3000 + waveform_data_buffer;
      TIM2->CCR1 += _Sys_Freq/waveform_sampleRate + 1;
      waveformFlag_newSample = 1;
    }

    //ARR flag -> reset waveform period
    if (TIM2->SR & TIM_SR_UIF) {
      TIM2->SR &= ~(TIM_SR_UIF); //clear flag
      TIM2->CCR1 = _Sys_Freq/waveform_sampleRate + 1;
      waveformFlag_newPeriod = 1;
    }
}


