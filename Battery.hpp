

#ifndef INCLUDED_BATTERY_HPP
#define INCLUDED_BATTERY_HPP

#include "stm32f4xx.h"

class Battery{
private:
	static float voltage;
	
public:
	static void initialize();

	static float getVoltage();

	static void rescanVoltage();

};
#endif
