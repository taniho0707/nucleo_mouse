
#ifndef INCLUDED_LED_HPP
#define INCLUDED_LED_HPP

#include "stm32f4xx.h"


class Led{
private:
	GPIO_TypeDef * gpio_port;
	uint16_t gpio_channel;

public:
	Led(GPIO_TypeDef *, uint16_t);

	void on();

	void off();

};

#endif
