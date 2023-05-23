#include "main.h"

#include "delay.h"
#include "keypad.h"
#include "lcd.h"
#include "DAC.h"

#include "waveform.h"

void SystemClock_Config(void);

int main(void)
{
  HAL_Init(); //reset of all peripherals, initializes the flash interface and the systick
  SystemClock_Config(); //configure the system clock

  //various inits
  SysTick_Init();
  keypad_init();
  LCD_init();

  //setup LCD
  LCD_command(LCD_COMMAND_CLEAR);
  LCD_command(LCD_COMMAND_HOME);
  LCD_command(LCD_COMMAND_DISPLAY_ON);

  //print test string to LCD
  char lcdString[2][16] = { "SAW 200 Hz  LAST","POSITIVE" };
  LCD_write_string(lcdString);

  waveform_init();

  //temp for testing
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
  GPIOC->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1);
  GPIOC->MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0);
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1);
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1);
  GPIOC->OSPEEDR |= (GPIO_OSPEEDR_OSPEED0 | GPIO_OSPEEDR_OSPEED1);
  GPIOC->ODR &= ~(GPIO_PIN_0 | GPIO_PIN_1);

  int func_type = 0; //0=squ 1=sin 2=saw
  int func_duty = 25; //%  <50 neg saw   >50 pos saw

  extern int waveformFlag_newPeriod;
  extern int waveformFlag_newSample;
  extern int waveform_clk_freq;
  extern int waveform_data_buffer;

  int sampleRate = 20000; //defined in the waveform.h, Hz
  int currSample = 0; //current sample number

  while (1){
	  if(waveformFlag_newPeriod){
		  waveformFlag_newPeriod = 0;
		  currSample = 0;
		  GPIOC->ODR |= GPIO_PIN_1; //testing
	  }
	  if(waveformFlag_newSample){
		  waveformFlag_newSample=0;

		  if(currSample==0) GPIOC->ODR &= ~GPIO_PIN_1; //testing

		  if(func_type==0){

			  //calc sample count to switch
			  int sampleSwitch = (sampleRate/waveform_clk_freq*func_duty/100);

			  if(currSample<sampleSwitch){
				  waveform_data_buffer = 4095; //testing
			  }
			  else{
				  waveform_data_buffer = 0;
			  }
		  }
		  currSample++;
	  }
  }
}


//system clock config
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) Error_Handler();
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) Error_Handler();
}

//error handler
void Error_Handler(void)
{
  __disable_irq();
  while (1);
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line){}
#endif
