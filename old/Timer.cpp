/**
 * @file Timer.cpp
 */

#include "Timer.h"


unsigned long long Timer::total = 0;
unsigned int Timer::waitnum = 0;


void Timer::start(){
	SYSTEM.MSTPCRA.BIT.MSTPA15 = 0;
	IEN(CMT0, CMI0) = 0;
	IPR(CMT0, CMI0) = 10;
	CMT0.CMCR.BIT.CKS = 2;		//PCLK/128
	CMT0.CMCR.BIT.CMIE = 1;		//割り込み許可
	CMT0.CMCNT = 0;
	CMT0.CMCOR = 390;
	IR(CMT0, CMI0) = 0;
	IEN(CMT0, CMI0) = 1;
	CMT.CMSTR0.BIT.STR0 = 1;
}

void Timer::stop(){
	CMT.CMSTR0.BIT.STR0 = 0;
}

void Timer::interrupt(){
	++total;
}

void Timer::wait(int i){
	waitnum = total;
	while(1){
		if(total - waitnum >= i){
			waitnum = 0;
			return;
		}
	}
}

