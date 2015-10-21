/**
 * @file Mouse.cpp
 */

#include "Mouse.h"





void Mouse::move(MouseMotion motion, SpeedParameter params){
	Motor::changeToStraight();
	switch(motion.currentMotion){
	/// @fixme とりあえず
	case EMotion_null:
		motion.speed = params.normal;
		Motor::setLinBothendsVelocity(motion.speed, 0, 0.09);
		break;
	case EMotion_Straight:
		if(motion.speed == 0) motion.speed = params.straight;
		Peripherals::setLED('B');
		if(motion.division) Motor::linTrapezoid(0.0894*motion.division, params.normal, params.normal, motion.speed);
		else Motor::linTrapezoid(0.18, motion.speed, motion.speed, motion.speed);
		break;
	case EMotion_Right90:
		motion.speed = 0.5;
		Motor::slalom(90, motion.speed);
		break;
	case EMotion_Left90:
		motion.speed = 0.5;
		Motor::slalom(-90, motion.speed);
		break;
	case EMotion_Turn180:
		Peripherals::setLED('R');
		Peripherals::addLED('B');
		Motor::setLinBothendsVelocity(0.5, 0.0, 0.09);
		Motor::pivotturn(3.1416);
		if(ENABLED_KABEATE){
			Motor::disable_integral();
			Motor::setLinVelocity(-0.2);
			Timer::wait(400);
			Motor::resetIntegral();
			Motor::resetGyroIntegral();
			Motor::setLinVelocity(0);
			Timer::wait(400);
			Motor::setSpeed(E_MotorLeft, 0);
			Motor::setSpeed(E_MotorRight, 0);
//			Sensor::resetGyro();
			Motor::resetIntegral();
			Motor::resetGyroIntegral();
			Motor::enable_integral();
			Motor::setLinBothendsVelocity(0, 0.5, 0.08+0.042);
		} else {
			Motor::setLinVelocity(0);
			Motor::setRadVelocity(0);
			Timer::wait(400);
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.5, 0.08);
		}
		Peripherals::setLED(' ');
		break;
	case EMotion_BigRight90:
		if(motion.speed == 0) motion.speed = params.turn90;
		if(motion.speed == 0.5){
			Motor::slalom(900, motion.speed);
		} else if(motion.speed == 0.8 || motion.speed == 1.0 || motion.speed == 1.5){
			Motor::slalom(900, 0.8);
		}
/*		else if(motion.speed == 1.5){
			Motor::setLinBothendsVelocity(1.5, 1.5, 0.09);
			Motor::slalom(900, 1.5);
			Motor::setLinBothendsVelocity(1.5, 1.5, 0.09);
		}
*/
		break;
	case EMotion_BigLeft90:
		if(motion.speed == 0) motion.speed = params.turn90;
		Motor::slalom(-900, motion.speed);
		break;
	case EMotion_BigRight180:
		if(motion.speed == 0) motion.speed = params.turn180;
		Motor::slalom(180, motion.speed);
		break;
	case EMotion_BigLeft180:
		if(motion.speed == 0) motion.speed = params.turn180;
		Motor::slalom(-180, motion.speed);
		break;
	case EMotion_Right45i:
		if(motion.speed == 0) motion.speed = params.turn45;
		Motor::slalom(45, motion.speed);
		break;
	case EMotion_Left45i:
		if(motion.speed == 0) motion.speed = params.turn45;
		Motor::slalom(-45, motion.speed);
		break;
	case EMotion_Right135i:
		if(motion.speed == 0) motion.speed = params.turn135;
		Motor::slalom(135, motion.speed);
		break;
	case EMotion_Left135i:
		if(motion.speed == 0) motion.speed = params.turn135;
		Motor::slalom(-135, motion.speed);
		break;
	case EMotion_Oblique:
		Motor::changeToOblique();
		if(motion.speed == 0) motion.speed = params.oblique;
//		Motor::setLinBothendsVelocity(motion.speed, motion.speed, 0.12728 * motion.division);
		Motor::linTrapezoid(0.12728 * motion.division, params.normal, params.normal, motion.speed);
		break;
	case EMotion_Right45o:
		if(motion.speed == 0) motion.speed = params.turn45;
		Motor::slalom(450, motion.speed);
		break;
	case EMotion_Left45o:
		if(motion.speed == 0) motion.speed = params.turn45;
		Motor::slalom(-450, motion.speed);
		break;
	case EMotion_Right135o:
		if(motion.speed == 0) motion.speed = params.turn135;
		Motor::slalom(1350, motion.speed);
		break;
	case EMotion_Left135o:
		if(motion.speed == 0) motion.speed = params.turn135;
		Motor::slalom(-1350, motion.speed);
		break;
	case EMotion_stop:
		Motor::setLinBothendsVelocity(motion.speed, 0, 0.09);
		break;
	case EMotion_ObliqueRight90:
		Motor::changeToOblique();
		if(motion.speed == 0) motion.speed = params.oblique90;
		Motor::slalom(907, motion.speed);
		break;
	case EMotion_ObliqueLeft90:
		Motor::changeToOblique();
		if(motion.speed == 0) motion.speed = params.oblique90;
		Motor::slalom(-907, motion.speed);
		break;
	case EMotion_OnlyTurn180:
//		Motor::pivotturn(3.2);
		Motor::pivotturn(3.1416);
//		Motor::setLinBothendsVelocity(0, motion.speed, 0.081);
		break;
	case EMotion_Turn180_None:
		Peripherals::setLED('R');
		Peripherals::addLED('B');
		Motor::setLinBothendsVelocity(0.5, 0.0, 0.09);
		Motor::pivotturn(3.1416);
		Motor::setLinVelocity(0);
		Motor::setRadVelocity(0);
		Timer::wait(400);
		Motor::resetIntegral();
		Motor::setLinBothendsVelocity(0, 0.5, 0.08);
		Peripherals::setLED(' ');
		break;
	case EMotion_Start:
		Motor::setLinBothendsVelocity(0, motion.speed, 0.04);
		break;
	default:
		Motor::setLinBothendsVelocity(motion.speed, 0, 0.09);
		break;
	}
}


void Mouse::moveOnce(EMotion emotion, SpeedParameter params, int division){
	float speed;
	if(emotion == EMotion_Straight || emotion == EMotion_Start) speed = params.straight;
	else if(emotion == EMotion_Oblique) speed = params.oblique;
	else speed = params.normal;
	MouseMotion motion = {
			emotion,
			division,
			speed,
			EMotion_null
	};
	move(motion, params);
}
