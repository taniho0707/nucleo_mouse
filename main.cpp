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
	// 	Usart::printf("FL:%d, L:%d, R:%d, FR:%d\n", SensorWall::getValue(E_LFront), SensorWall::getValue(E_Left), SensorWall::getValue(E_Right), SensorWall::getValue(E_RFront));
	// 	Timer::wait_ms(100);
	// }

	Motor::excitate();
	while(Switch::isPushing());
	user_led.on();
	Timer::wait_ms(1000);
	// Motor::pulseRun(100, 630);
	// Motor::pulseTurn(20, 360*3);
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

