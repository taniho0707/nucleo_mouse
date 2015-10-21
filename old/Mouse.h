/**
 * @file Mouse.h
 */
#ifndef INCLUDED_MOUSE_H
#define INCLUDED_MOUSE_H

#include "const.h"
#include "Motor.h"


/**
 * マウスの動作を抽象化するクラスです
 */
class Mouse{
private:



public:


	static void straight();


	static void turnRight();


	static void turnLeft();


	static void pivot();


	/**
	 * @todo 最大速ターン速に対応
	 */
	static void move(MouseMotion motion, SpeedParameter params);

	static void moveOnce(EMotion emotion, SpeedParameter params, int division);

};



#endif
