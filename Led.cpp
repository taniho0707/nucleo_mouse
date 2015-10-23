

#include "Led.hpp"


Led::Led(GPIO_TypeDef * channel, uint16_t port){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = port;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(channel, &GPIO_InitStructure);
	gpio_channel = port;
	gpio_port = channel;
}

void Led::on(){
	GPIO_SetBits(gpio_port, gpio_channel);
}

void Led::off(){
	GPIO_ResetBits(gpio_port, gpio_channel);
}

