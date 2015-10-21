/* Includes ------------------------------------------------------------------*/
#include "main.hpp"
#include <stdio.h>
#include <stdarg.h>

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

void initTIM(){
	TIM_TimeBaseInitTypeDef TIM_initstr;
	NVIC_InitTypeDef NVIC_initstr;
	TIM_OCInitTypeDef TIM_OC_initstr;
	GPIO_InitTypeDef GPIO_initstr;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_initstr.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_6;
	GPIO_initstr.GPIO_Mode = GPIO_Mode_AF;
	GPIO_initstr.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_initstr.GPIO_OType = GPIO_OType_PP;
	GPIO_initstr.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_initstr);

	// とりあえずの周波数(1kHz)
	TIM_initstr.TIM_Period = 420-1;
	TIM_initstr.TIM_Prescaler = 200-1;
	TIM_initstr.TIM_ClockDivision = 0;
	TIM_initstr.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_initstr.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_initstr);
	TIM_TimeBaseInit(TIM3, &TIM_initstr);

	TIM_OC_initstr.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_initstr.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC_initstr.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_initstr.TIM_Pulse = 210-1;
	TIM_OC1Init(TIM2,&TIM_OC_initstr);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Disable);
	TIM_OC1Init(TIM3,&TIM_OC_initstr);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Disable);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);

	NVIC_initstr.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_initstr.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_initstr.NVIC_IRQChannelSubPriority = 1;
	NVIC_initstr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_initstr);
	NVIC_initstr.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_Init(&NVIC_initstr);

	TIM_TimeBaseInit(TIM2, &TIM_initstr);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_TimeBaseInit(TIM3, &TIM_initstr);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void initLED(){
	GPIO_InitTypeDef GPIO_InitStructure;
/* Enable the GPIOA peripheral */ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
/* Configure MCO1 pin(PA5) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void initMotorPin(){
	GPIO_InitTypeDef GPIO_InitStructure;
/* Enable the GPIOA peripheral */ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
/* Configure MCO1 pin(PA5) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void initUART(){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_USART6);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200*3;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure);
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART6, ENABLE);
}

void USART_Send1byte(USART_TypeDef *USARTx, char Data){
	USART_SendData(USARTx, (uint16_t)Data);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
}

void USART_Sendnbyte(USART_TypeDef *USARTx, char *Data, int len){
	int i;
	for(i=0; i<len; ++i) USART_Send1byte(USARTx, Data[i]);
}

void USART_myprintf(USART_TypeDef *USARTx, const char *fmt, ...){
	static char buffer[100];
	int len = 0;
	va_list ap;
	va_start(ap, fmt);
	len = vsprintf(buffer, fmt, ap);
	USART_Sendnbyte(USARTx, buffer, len);
	va_end(ap);
	return len;
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
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 960);//2400);
	
	initLED();
	initMotorPin();
	initUART();
	// initADC();
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	// USART_myprintf(USART2, "Hello!\n");
	Delay(1000);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	Delay(1000);
	GPIO_SetBits(GPIOA, GPIO_Pin_10);
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	initTIM();

	volatile uint16_t tmp = 0;
	while (1){
		if(tmp){
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			tmp = 0;
		} else {
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			tmp = 1;
		}
		Delay(500);
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime){
	uwTimingDelay = nTime;

	while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void){
	if (uwTimingDelay != 0x00){
		uwTimingDelay--;
	}
}

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

