#include "main.hpp"
#include <stdio.h>
#include <stdarg.h>

RCC_ClocksTypeDef RCC_Clocks;




int main(void){
	SystemInit();
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency/840);

	Motor::initialize();
	Usart::initialize();
	Switch::initialize();
	Battery::initialize();
	SensorWall::initialize();

	Led user_led(GPIOA, GPIO_Pin_5);

	user_led.on();
	// GPIO_ResetBits(GPIOB, GPIO_Pin_7); // SensorLED
	Timer::wait_ms(1000);
	user_led.off();
	Timer::wait_ms(1000);

	Usart::printf("Hello, nucleo mouse!\n");
	Battery::rescanVoltage();
	Usart::printf("\tVoltage: %f V\n", Battery::getVoltage());
	if(Battery::getVoltage() < 6.8){
		while(1){
			user_led.on();
			Timer::wait_ms(100);
			user_led.off();
			Timer::wait_ms(100);
		}
	}

	SensorWall::start();
	Timer::wait_ms(500);

	// while(1){
	// 	Usart::printf("FL:%4d, L:%4d, R:%4d, FR:%4d\n", SensorWall::getValue(E_LFront), SensorWall::getValue(E_Left), SensorWall::getValue(E_Right), SensorWall::getValue(E_RFront));
	// 	Timer::wait_ms(100);
	// }

	Motor::excitate();
	while(Switch::isPushing());
	user_led.on();
	Timer::wait_ms(1000);
	user_led.off();
	Motor::pulseRun(50, 6300);
	// Motor::startRotate();
	// TIM_TimeBaseInitTypeDef TIM_initstr;
	// TIM_OCInitTypeDef TIM_OC_initstr;
	// TIM_initstr.TIM_Period = 12500 - 1;
	// TIM_initstr.TIM_Prescaler = 100 - 1;
	// TIM_initstr.TIM_ClockDivision = 0;
	// TIM_initstr.TIM_CounterMode = TIM_CounterMode_Up;
	// TIM_OC_initstr.TIM_OCMode = TIM_OCMode_PWM1;
	// TIM_OC_initstr.TIM_OCPolarity = TIM_OCPolarity_High;
	// TIM_OC_initstr.TIM_OutputState = TIM_OutputState_Enable;
	// TIM_OC_initstr.TIM_Pulse = 6250 - 1;
	// TIM_TimeBaseInit(TIM3, &TIM_initstr);
	// TIM_OC1Init(TIM3,&TIM_OC_initstr);
	// TIM_TimeBaseInit(TIM2, &TIM_initstr);
	// TIM_OC1Init(TIM2,&TIM_OC_initstr);
	// Timer::wait_ms(2000);
	// Motor::setSpeed(E_MotorLeft, 0);
	// Motor::setSpeed(E_MotorRight, 0);
	// Motor::stopRotate();
	// Motor::pulseTurn(140, 3600);
	Timer::wait_ms(1000);
	Motor::disexcitate();

	volatile uint16_t tmp = 0;
	while (1){
		if(tmp){
			user_led.on();
			tmp = 0;
		} else {
			user_led.off();
			tmp = 1;
		}
		Timer::wait_ms(500);
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

