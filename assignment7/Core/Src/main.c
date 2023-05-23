#include "main.h"
#include <stdio.h>

#include "ADC.h"
#include "uart.h"

void SystemClock_Config(void);

int main(void)
{
  HAL_Init(); //reset of all peripherals, initializes the flash interface and the systick
  SystemClock_Config(); //configure the system clock

  LPUART_init();
  LPUART_print("sys init");

  ADC_init();

  extern int ADC_lastVal;
  extern int ADC_newValFlag;

  LPUART_print("starting");

  while (1)
  {
	  if(ADC_newValFlag){
		  ADC_newValFlag=0;//clear flag

		  char voltageStr[16];
		  sprintf(voltageStr, "%d", ADC_lastVal);
		  LPUART_print(voltageStr);
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
