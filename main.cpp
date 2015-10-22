#include "main.hpp"
#include <stdio.h>
#include <stdarg.h>

RCC_ClocksTypeDef RCC_Clocks;


void initLED(){
	GPIO_InitTypeDef GPIO_InitStructure;
/* Enable the GPIOA peripheral */ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
/* Configure MCO1 pin(PA5) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}




// void initADC(){
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	ADC_InitTypeDef ADC_InitStructure;
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

// 	GPIO_StructInit(&GPIO_InitStructure);
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);

// 	ADC_Cmd(ADC1, DISABLE);
// 	ADC_DeInit();

// 	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
// 	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
// 	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
// 	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
// 	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
// 	ADC_InitStructure.ADC_NbrOfConversion = 1;
// 	ADC_Init(ADC1, &ADC_InitStructure);
// 	ADC_Cmd(ADC1, ENABLE);
// 	Delay(100);
// }

// uint16_t getAD1_10(void){
// 	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_56Cycles);
// 	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
//     ADC_SoftwareStartConv(ADC1);
//     while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);
// 	GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
// 	uint16_t retval = ADC_GetConversionValue(ADC1);
//     return retval;
// }


int main(void){
	SystemInit();
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency/840);

	Motor::initialize();
	Usart::initialize();
	Switch::initialize();

	initLED();
	// initADC();
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	// GPIO_ResetBits(GPIOB, GPIO_Pin_7); // SensorLED
	// USART_myprintf(USART2, "Hello!\n");
	Timer::wait_ms(1000);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	Timer::wait_ms(1000);

	Usart::printf("Hello, nucleo mouse!\n");

	Motor::excitate();
	while(Switch::isPushing());
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	Timer::wait_ms(1000);
	Motor::pulseRun(100, 630);
	Timer::wait_ms(1000);
	Motor::disexcitate();

	volatile uint16_t tmp = 0;
	while (1){
		if(tmp){
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			tmp = 0;
		} else {
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			tmp = 1;
		}
		Timer::wait_ms(500);
	}
}

// /**
//   * @brief  Inserts a delay time.
//   * @param  nTime: specifies the delay time length, in milliseconds.
//   * @retval None
//   */
// void Delay(__IO uint32_t nTime){
// 	uwTimingDelay = nTime;

// 	while(uwTimingDelay != 0);
// }

// /**
//   * @brief  Decrements the TimingDelay variable.
//   * @param  None
//   * @retval None
//   */
// void TimingDelay_Decrement(void){
// 	if (uwTimingDelay != 0x00){
// 		uwTimingDelay--;
// 	}
// }

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line){
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1){
	}
}
#endif

