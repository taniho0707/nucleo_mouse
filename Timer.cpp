/*
 * @file Timer.cpp
 */

#include "Timer.hpp"

__IO int32_t Timer::total = 0;
__IO int32_t Timer::waitnum = 0;


void Timer::start(){
	TIM_TimeBaseInitTypeDef TIM_initstr;
	NVIC_InitTypeDef NVIC_initstr;

	
}

void Timer::stop(){
	
}

void Timer::wait_ms(__IO int32_t t){
	waitnum = t;
	while(waitnum != 0x00);
}

void Timer::wait_ms_decrement(){
	if(waitnum != 0x00) --waitnum;
	++ total;
}

void Timer::interrupt(){
	
}
