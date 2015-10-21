/**
 * @file DC_mouse03.cpp
 * @mainpage 2015年5月18日より作成をはじめたマウス用プログラム。<br>
 * 今までより扱いやすく，アルゴリズムや調整に専念しやすいようなシステムを目指しています。
 */


#include "typedefine.h"
#include "iodefine.h"
#include "const.h"
#include "parameters.h"

#include "stdio.h"
#include "stdarg.h"

#include "r_flash_api_rx\r_flash_api_rx_if.h"
#include "CExtern.h"

#include "Timer.h"
#include "Encoder.h"
#include "Sensor.h"
#include "Control.h"

#include "Peripherals.h"


void main(void);
#ifdef __cplusplus
extern "C" { void abort(void); }
#endif


void interrupt_CMT0(){
	Timer::interrupt();
	Encoder::interrupt();
	Sensor::interrupt();
	Motor::interrupt();
}

void interrupt_CMT1(){

}

void interrupt_CMT2(){

}

void interrupt_S12AD0(){

}


void main(void){
	SYSTEM.SCKCR.BIT.PCK = 1;
	SYSTEM.SCKCR.BIT.ICK = 0;

	Datalog::setup();
//	R_FlashDataAreaBlankCheck(0x00100000, BLANK_CHECK_8_BYTE);

	while(true){
//		Control::test();
		Control::modeselect();
	}
}

#ifdef __cplusplus
void abort(void){
}
#endif
