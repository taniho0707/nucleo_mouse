
#ifndef INCLUDED_SENSORWALL_HPP
#define INCLUDED_SENSORWALL_HPP

#include "stm32f4xx.h"
#include "const.hpp"
#include "parameters.hpp"
#include "Map.hpp"


class SensorWall{
private:
	static float current_value[4];
	static char is_working;
	static int ref_straight_value[4];
	static int thr_straight_value[4];

	SensorWall();

public:
	static void initialize();

	static void start();

	static void stop();

	static void interrupt();

	static void onLed();

	static void offLed();


	static int getValue(ESensorPosition);

	static int existWall(ESensorPosition);

	static Walldata getWall();

	static int getCorrection(int max);

};

#endif
