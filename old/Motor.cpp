/**
 * @file Motor.cpp
 */

#include "Motor.h"


float Motor::param_ideal = GAIN_IDEAL*141.42857*WEIGHT;

int Motor::working = 0;

float Motor::currentLinVelocity = 0;
float Motor::currentRadVelocity = 0;

float Motor::idealTargetLinVelocityR = 0;
float Motor::idealTargetLinVelocityL = 0;
float Motor::targetVelocityL = 0;
float Motor::targetVelocityR = 0;
float Motor::targetLinVelocity = 0;
float Motor::targetRadVelocity = 0;
float Motor::targetLinAccel = ACCEL;
float Motor::targetRadAccel = RADACCEL;

float Motor::targetDistance = 0;

int Motor::enable_ideal = 0;
int Motor::enable_wall = 1;
int Motor::enable_wall_integral = 1;
int Motor::enable_velocity = 1;
int Motor::enable_rad = 1;
int Motor::enable_gyro = 1;
int Motor::enable_gyro_integral = 1;
int Motor::enable_distance = 0;
int Motor::enable_sensor = 0;

float Motor::integralLin = 0;
float Motor::integralRad = 0;
float Motor::integralWall = 0;
float Motor::integral_lin_encoder = 0;
float Motor::integral_rad_gyro = 0;
float Motor::integral_wall = 0;

unsigned char Motor::isOblique = 0;

int Motor::dead = 0;


int Motor::abs(int i){
	if(i >= 0) return i;
	else return -i;
}

float Motor::absf(float i){
	if(i > 0) return i;
	else return -i;
}


void Motor::start(){
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;

	MTU.TSTRA.BIT.CST4 = 0;
	MTU.TSTRB.BIT.CST7 = 0;

	//MTIO4A，B，左モーター(とりあえずAにPWM出力)
	MTU.TOERA.BIT.OE4A = 1;
	MTU4.TCR.BIT.CCLR = 2;
	MTU4.TCR.BIT.TPSC = 0;
	MTU4.TCR.BIT.CKEG = 0;
	MTU4.TMDR1.BIT.MD = 2;
	MTU4.TIORH.BIT.IOA = 5;
	MTU4.TIORH.BIT.IOB = 2;
	MTU4.TGRA = 0;
	MTU4.TGRB = 256;
	MTU4.TCNT = 0;

	//MTIO2A，B，右モーター(とりあえずAにPWM出力)
	MTU.TOERB.BIT.OE7A = 1;
	MTU7.TCR.BIT.CCLR = 2;
	MTU7.TCR.BIT.TPSC = 0;
	MTU7.TCR.BIT.CKEG = 0;
	MTU7.TMDR1.BIT.MD = 2;
	MTU7.TIORH.BIT.IOA = 5;
	MTU7.TIORH.BIT.IOB = 2;
	MTU7.TGRA = 0;
	MTU7.TGRB = 256;
	MTU7.TCNT = 0;

	PORT7.DDR.BIT.B3 = 1;
	PORT7.DR.BIT.B3 = 0;
	PORT7.DDR.BIT.B2 = 1;
	PORT7.DR.BIT.B2 = 0;
	PORT9.DDR.BIT.B3 = 1;
	PORT9.DR.BIT.B3 = 0;
	PORT9.DDR.BIT.B4 = 1;
	PORT9.DR.BIT.B4 = 0;

	MTU.TSTRA.BIT.CST4 = 1;
	MTU.TSTRB.BIT.CST7 = 1;
	working = 1;
}

void Motor::stop(){
	setSpeed(E_MotorLeft, 0);
	setSpeed(E_MotorRight, 0);
	working = 0;
}

void Motor::setSpeed(EMotorPosition side, int duty){
	if(working){
		int tmp = abs(duty);
		tmp = (int)(6.8 * tmp / Sensor::getBattery());
		if(tmp > 255) tmp = 255;
#ifdef SPEEDLIMIT_MODE
		if(tmp > 170) tmp = 170;
#endif
		if(duty >= 0){
			if(side == E_MotorLeft){
				PORT7.DR.BIT.B3 = 0;
				MTU4.TGRA = tmp;
			} else {
				PORT9.DR.BIT.B3 = 0;
				MTU7.TGRA = tmp;
			}
		} else {
			if(side == E_MotorLeft){
				PORT7.DR.BIT.B3 = 1;
				MTU4.TGRA = 255 - tmp;
			} else {
				PORT9.DR.BIT.B3 = 1;
				MTU7.TGRA = 255 - tmp;
			}
		}
	} else {
		if(side == E_MotorLeft){
			PORT7.DR.BIT.B3 = 0;
			MTU4.TGRA = 0;
		} else {
			PORT9.DR.BIT.B3 = 0;
			MTU7.TGRA = 0;
		}
	}
}

void Motor::interrupt(){
	if(working == 0){
		setSpeed(E_MotorLeft, 0);
		setSpeed(E_MotorRight, 0);
		return;
	}

	// データログ関係のローカル変数
	static long dfCount = 0;

	// フェールセーフ
	if(absf(integral_lin_encoder) > 100 || absf(integral_rad_gyro) > 3000){
		targetLinVelocity = 0;
		targetRadVelocity = 0;
		resetIntegral();
		Peripherals::addLED('R');
		Peripherals::addLED('G');
		Peripherals::addLED('B');
		setSpeed(E_MotorLeft, 0);
		setSpeed(E_MotorRight, 0);
		dead = 1;
		while(Peripherals::getSwitch() == 0){
			stop();
		}
		return;
	}

	/// @todo 積分値のリセット
//	if(targetRadVelocity == 0) integralRad = 0;
//	if(enable_wall_integral == 0) integralWall = 0;

	// 一時保存用変数 詳細はプリント参照
	static float target_motor_lin_power = 0;
	static float target_motor_rad_power = 0;
	static float target_motor_r_power = 0;
	static float target_motor_l_power = 0;
	static float target_vel_rev = 0;
	static float target_rad_rev = 0;

	// 壁積分値の計算
	integral_wall += Sensor::getCorrection(MAX_WALL_CORRECTION);

	// linear成分の計算
	target_vel_rev = targetLinVelocity - (Encoder::getVelocity(E_MotorRight) + Encoder::getVelocity(E_MotorLeft))/2;
	integral_lin_encoder += target_vel_rev;
	target_motor_lin_power = (float)(GAIN_ENCODER) * target_vel_rev + GAIN_INTEGRAL * integral_lin_encoder;

	// rotation成分の計算
	target_rad_rev = (targetRadVelocity + enable_wall * GAIN_KABE * Sensor::getCorrection(MAX_WALL_CORRECTION)) - Sensor::getGyroRad();
	integral_rad_gyro += target_rad_rev;
	target_motor_rad_power = (float)(GAIN_GYRO) * enable_gyro * target_rad_rev + GAIN_GYRO_INTEGRAL * integral_rad_gyro * enable_gyro_integral/* + enable_wall * GAIN_KABE * Sensor::getCorrection(MAX_WALL_CORRECTION) + enable_wall_integral * GAIN_KABE_INTEGRAL * integral_wall*/;

	// モーター出力
	target_motor_r_power = target_motor_lin_power + target_motor_rad_power;
	target_motor_l_power = target_motor_lin_power - target_motor_rad_power;
	setSpeed(E_MotorRight, (int)( target_motor_r_power ) );
	setSpeed(E_MotorLeft, (int)( target_motor_l_power ) );

	/// @todo 迷路も保存させたい
	// データログ保存処理
	if(dfCount > 9){
//		Datalog::saveData((int)(targetLinVelocity*1000), (int)(Encoder::getVelocity(E_MotorLeft)*1000), (int)(targetRadVelocity*1000), (int)(Encoder::getVelocity(E_MotorRight)*1000));
//		Datalog::saveData(Sensor::existWall(E_LFront), Sensor::existWall(E_Right), Sensor::existWall(E_Left), Sensor::existWall(E_RFront));
		Datalog::saveData((int)(targetLinVelocity*1000), (int)(Encoder::getVelocity(E_MotorLeft)*1000), Sensor::getBattery()*1000, (int)(Encoder::getVelocity(E_MotorRight)*1000));
//		Datalog::saveData((int)(Sensor::getGyroRad() * 1000), (int)(integral_rad_gyro * 1000), (int)(targetRadVelocity * 1000), 0);
//		Datalog::saveData((int)(idealTargetLinVelocityL*1000), (int)(enable_velocity * GAIN_ENCODER_L * difL + targetVelocityL + GAIN_INTEGRAL_L * integralL), (int)(idealTargetLinVelocityR*1000), (int)(enable_velocity * GAIN_ENCODER_R * difR + targetVelocityR + GAIN_INTEGRAL_R * integralR));
		dfCount = 0;
	} else {
		++ dfCount;
	}
}


void Motor::setLinAccel(float a){
	targetLinAccel = a;
}

void Motor::setLinVelocity(float v){
	targetLinVelocity = v;
}


void Motor::setRadAccel(float a){
	targetRadAccel = a;
}

void Motor::setRadVelocity(float v){
	targetRadVelocity = v;
}


void Motor::disable_integral(){
	enable_wall_integral = 0;
	enable_gyro_integral = 0;
}

void Motor::enable_integral(){
	enable_wall_integral = 1;
	enable_gyro_integral = 1;
}

void Motor::resetIntegral(){
	integralLin = 0;
	integralRad = 0;
	integralWall = 0;
	integral_lin_encoder = 0;
	integral_rad_gyro = 0;
}

void Motor::resetGyroIntegral(){
	integralRad = 0;
}

void Motor::changeToOblique(){
	isOblique = 1;
}

void Motor::changeToStraight(){
	isOblique = 0;
}

unsigned char Motor::isCurrentOblique(){
	return isOblique;
}


float Motor::setLinBothendsVelocity(float startvl, float endvl, float distance){
	float keepdistance = 0;
	enable_wall = 1;
	enable_wall_integral = 1;
	enable_rad = 0;
	enable_gyro = 1;
	enable_gyro_integral = 0;
	targetLinVelocity = startvl;
	targetRadVelocity = 0;
//	integral_rad_gyro = 0;
	float totaldistance = 0;

	if(startvl > endvl){
		// 減速
		keepdistance = distance - ((startvl*startvl - endvl*endvl)/2/targetLinAccel)/* - 0.05*/;
		while(totaldistance < keepdistance){
			targetLinVelocity = startvl;
			totaldistance += 0.001 * targetLinVelocity;
			Timer::wait(1);
		}
		while(targetLinVelocity > endvl){
			targetLinVelocity -= 0.001 * targetLinAccel;
			if(endvl < 0.1 && targetLinVelocity < 0.1){
				break;
			}
			totaldistance += 0.001 * targetLinVelocity;
			Timer::wait(1);
		}
		while(totaldistance < distance){
			if(ENABLED_MAEKABE && endvl == 0 && Sensor::existWall(E_LFront) && Sensor::existWall(E_RFront)){
				Peripherals::addLED('R');
				float tmp = 0;
				///@todo 一時的に1秒動作にしてる
				do {
//					for(int j=0; j<1000; ++j){
					tmp = (float)(1.0*Sensor::getFrontCorrection()/12000);
					if(tmp > 0.3) tmp = 0.3;
					else if(tmp < -0.3) tmp = -0.3;
					targetLinVelocity = tmp;
					Timer::wait(1);
//					}
				} while(abs(Sensor::getFrontCorrection()) > 8);	/// @todo 数値
				Peripherals::setLED(' ');
				break;
			} else if(endvl == 0){
				targetLinVelocity = 0.1;
			} else {
				targetLinVelocity = endvl;
			}
			totaldistance += 0.001 * targetLinVelocity;
			Timer::wait(1);
		}
		targetLinVelocity = endvl;
		return totaldistance;
	} else if(startvl == endvl){
		// 維持
		unsigned int count = (int)(1000.0*distance/startvl);
		for(int i=0; i<count; ++i){
			targetLinVelocity = endvl;
			Timer::wait(1);
		}
		return distance;
	} else {
		// 加速
		if(startvl == 0){
			enable_wall = 0;
			enable_wall_integral = 0;
			enable_gyro = 0;
			enable_gyro_integral = 0;
		}
		unsigned char now_gyro_turnedon = 0;
		while(targetLinVelocity < endvl){
			targetLinVelocity += 0.001 * targetLinAccel;
			if(targetLinVelocity > endvl){
				targetLinVelocity = endvl;
			}
			totaldistance += 0.001 * targetLinVelocity;
			Timer::wait(1);
			if(targetLinVelocity >= 0.5/** @todo 値調整 */ && (now_gyro_turnedon == 0)){
				enable_wall = 1;
				enable_wall_integral = 1;
				enable_gyro = 1;
				enable_gyro_integral = 1;
				integral_lin_encoder = 0;
				integral_rad_gyro = 0;
				now_gyro_turnedon = 1;
			}
		}
		enable_wall = 1;
		enable_wall_integral = 1;
		targetLinVelocity = endvl;
		while(totaldistance < distance){
			totaldistance += 0.001 * targetLinVelocity;
			Timer::wait(1);
		}
		return totaldistance;
	}
}

float Motor::setRadBothendsVelocity(float startvl, float endvl, float degree){
	float totaldegree = 0;
	float keepdegree = 0;
	targetRadVelocity = startvl;
	while(1){
		if(degree > 0){		//左回り(正)
			if(startvl > endvl){		//減速													//減速動作である場合
				keepdegree = degree - ((startvl*startvl - endvl*endvl)/2/targetRadAccel)/* - 0.05*/;
				while(totaldegree < keepdegree){
					targetRadVelocity = startvl;
					totaldegree += 0.001 * Sensor::getGyroRad();
					Timer::wait(1);
				}
				while(targetRadVelocity > endvl){
					targetRadVelocity -= 0.001 * targetRadAccel;
					totaldegree += 0.001 * Sensor::getGyroRad();
					Timer::wait(1);
				}
				while(totaldegree < degree){
//					if(endvl == 0){
//						targetRadVelocity = 0.1;
//					} else {
						targetRadVelocity = endvl;
//					}
					totaldegree += 0.001 * Sensor::getGyroRad();
					Timer::wait(1);
				}
				targetRadVelocity = endvl;
				return totaldegree;
			} else {																			//加速動作である場合
				while(targetRadVelocity < endvl){
					targetRadVelocity += 0.001 * targetRadAccel;
					totaldegree += 0.001 * Sensor::getGyroRad();
					Timer::wait(1);
				}
				targetRadVelocity = endvl;
				return totaldegree;
			}
		} else {		//右回り(負)
			if(startvl > endvl){		//減速													//減速動作である場合
				keepdegree = absf(degree) - ((startvl*startvl - endvl*endvl)/2/targetRadAccel)/* - 0.05*/;
				while(absf(totaldegree) < keepdegree){
					targetRadVelocity = -startvl;
					totaldegree += 0.001 * Sensor::getGyroRad();
					Timer::wait(1);
				}
				while(absf(targetRadVelocity) < endvl){
					targetRadVelocity -= 0.001 * targetRadAccel;
					totaldegree += 0.001 * Sensor::getGyroRad();
					Timer::wait(1);
				}
				while(absf(totaldegree) < absf(degree)){
//					if(endvl == 0){
//						targetRadVelocity = -0.1;
//					} else {
						targetRadVelocity = -endvl;
//					}
					totaldegree += 0.001 * Sensor::getGyroRad();
					Timer::wait(1);
				}
				targetRadVelocity = -endvl;
				return totaldegree;
			} else {																			//加速動作である場合
				while(absf(targetRadVelocity) < endvl){
					targetRadVelocity -= 0.001 * targetRadAccel;
					totaldegree += 0.001 * Sensor::getGyroRad();
					Timer::wait(1);
				}
				targetRadVelocity = -endvl;
				return totaldegree;
			}
		}
	}
}


void Motor::linTrapezoid(float distance, float startvl, float endvl, float maxSpeed){
	float accel = ACCEL;
	float maxv = sqrtf(distance*accel + startvl*startvl);
	Peripherals::setLED(' ');
	setLinBothendsVelocity(startvl, ((maxv < maxSpeed) ? maxv : maxSpeed), distance/2.0);
	integral_rad_gyro = 0;
	Peripherals::setLED('R');
	setLinBothendsVelocity(((maxv < maxSpeed) ? maxv : maxSpeed), endvl, (distance/2.0));
}

void Motor::pivotturn(float deg){
	float tmp = 0;
	float maxSpeed = 6.2832;

	enable_wall = 0;
	enable_wall_integral = 0;
	enable_rad = 1;
	enable_gyro = 1;
	enable_gyro_integral = 1;
	targetLinVelocity = 0;
	tmp = setRadBothendsVelocity(0, maxSpeed, deg);
	setRadBothendsVelocity(maxSpeed, 0, deg - tmp);
	targetLinVelocity = 0;
	integralRad = 0;
	resetIntegral();
}


void Motor::slalom(int mode, float v){
	float time_i = 0.001;
	float v_line = v, v_theta = 0;
	float a_theta = 0;
	float x = 90, y = 180, theta = 0;
	volatile float keep_before = 0, keep_after = 0;
	float max_theta = 3.1416 / 6;
	float allround = 0;
	int slalomcount = 0;

	enable_rad = 0;
	enable_gyro = 1;
	enable_gyro_integral = 1;
	enable_wall = 0;
	enable_wall_integral = 0;

	if(v == 0.5){
		targetLinVelocity = 0.5;
		targetRadVelocity = 0.0;
		if(mode == -90){
			a_theta = 250;
			v_line = 0.5;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2 * 1.03;//1.1;
			keep_after = 0.052;
			keep_before = 0.016;
		} else if(mode == 90){
			a_theta = -250;
			v_line = 0.5;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2 * 1.0;//0.985;
			keep_after = 0.052;
			keep_before = 0.016;
		} else if(mode == 900){
			a_theta = -27;
			v_line = 0.5;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2 * 1.0;
			keep_after = 0.045;
			keep_before = 0.0;
		} else if(mode == -900){
			a_theta = 27;
			v_line = 0.5;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2 * 1.0;
			keep_after = 0.054;
			keep_before = 0.0;
		} else if(mode == 180){
			a_theta = -20.5;
			v_line = 0.5;
			max_theta = 3.1416 / 3;
			allround = 3.1416 * 1.0;
			keep_after = 0.0;
			keep_before = 0.0;
		} else if(mode == -180){
			a_theta = 20.5;
			v_line = 0.5;
			max_theta = 3.1416 / 3;
			allround = 3.1416 * 1.0;
			keep_after = 0.0;
			keep_before = 0.0;
		} else if(mode == 45){
			a_theta = -35;
			v_line = 0.5;
			max_theta = 3.1416 / 12;
			allround = 3.1416 / 4 * 1.0;
			keep_after = 0.04;
			keep_before = 0.01;
		} else if(mode == -45){
			a_theta = 35;
			v_line = 0.5;
			max_theta = 3.1416 / 12;
			allround = 3.1416 / 4 * 1.0;
			keep_after = 0.04;
			keep_before = 0.01;
		} else if(mode == 450){
			a_theta = -35;
			v_line = 0.5;
			max_theta = 3.1416 / 12;
			allround = 3.1416 / 4 * 1.0;
			keep_after = 0.03;
			keep_before = 0.04;
		} else if(mode == -450){
			a_theta = 35;
			v_line = 0.5;
			max_theta = 3.1416 / 12;
			allround = 3.1416 / 4 * 1.0;
			keep_after = 0.03;
			keep_before = 0.04;
		} else if(mode == 135){
			a_theta = -44;
			v_line = 0.5;
			max_theta = 3.1416 * 2 / 9 * 1.0;
			allround = 3.1416 * 3 / 4;
			keep_after = 0.038;
			keep_before = 0.052;
		} else if(mode == -135){
			a_theta = 44;
			v_line = 0.5;
			max_theta = 3.1416 * 2 / 9 * 1.0;
			allround = 3.1416 * 3 / 4;
			keep_after = 0.038;
			keep_before = 0.052;
		} else if(mode == 1350){
			a_theta = -44;
			v_line = 0.5;
			max_theta = 3.1416 * 2 / 9 * 1.0;
			allround = 3.1416 * 3 / 4;
			keep_after = 0.05;
			keep_before = 0.038;
		} else if(mode == -1350){
			a_theta = 44;
			v_line = 0.5;
			max_theta = 3.1416 * 2 / 9 * 1.0;
			allround = 3.1416 * 3 / 4;
			keep_after = 0.05;
			keep_before = 0.038;
		} else if(mode == 907){
			a_theta = -50;
			v_line = 0.5;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2 * 1.0;
			keep_after = 0.02;
			keep_before = 0.02;
		} else if(mode == -907){
			a_theta = 50;
			v_line = 0.5;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2 * 1.0;
			keep_after = 0.02;
			keep_before = 0.02;
		}
	} else if(v == 0.8){
		if(mode == -90){
			targetLinVelocity = 0.8;
			targetRadVelocity = 0;
			a_theta = 98;
			v_line = 0.8;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2;
			keep_after = 0.0;
			keep_before = 0.0;
		} else if(mode == 90){
			targetLinVelocity = 0.8;
			targetRadVelocity = 0;
			a_theta = -95;
			v_line = 0.8;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2;
			keep_after = 0.0;
			keep_before = 0.0;
		} else if(mode == 900){
			targetLinVelocity = 0.8;
			targetRadVelocity = 0;
			a_theta = -46;
			v_line = 0.8;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2;
			keep_after = 0.0;
			keep_before = 0.0;
		} else if(mode == -900){
			targetLinVelocity = 0.8;
			targetRadVelocity = 0;
			a_theta = 46;
			v_line = 0.8;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2;
			keep_after = 0.0;
			keep_before = 0.0;
		} else if(mode == 180){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = -25.5;
			v_line = 0.5;
			max_theta = 3.1416 / 3;
			allround = 3.1416 * 1.0;
			keep_after = 0.0;
			keep_before = 0.0;
		} else if(mode == -180){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = 25.5;
			v_line = 0.5;
			max_theta = 3.1416 / 3;
			allround = 3.1416 * 1.0;
			keep_after = 0.0;
			keep_before = 0.0;
		} else if(mode == 45){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = -45;
			v_line = 0.5;
			max_theta = 3.1416 / 12;
			allround = 3.1416 / 4 * 1.0;
			keep_after = 0.045;
			keep_before = 0.01;
		} else if(mode == -45){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = 45;
			v_line = 0.5;
			max_theta = 3.1416 / 12;
			allround = 3.1416 / 4 * 1.0;
			keep_after = 0.045;
			keep_before = 0.01;
		} else if(mode == 450){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = -45;
			v_line = 0.5;
			max_theta = 3.1416 / 12;
			allround = 3.1416 / 4 * 1.0;
			keep_after = 0.03;
			keep_before = 0.04;
		} else if(mode == -450){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = 45;
			v_line = 0.5;
			max_theta = 3.1416 / 12;
			allround = 3.1416 / 4 * 1.0;
			keep_after = 0.03;
			keep_before = 0.04;
		} else if(mode == 135){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = -51;
			v_line = 0.5;
			max_theta = 3.1416 * 2 / 9;
			allround = 3.1416 * 3 / 4;
			keep_after = 0.038;
			keep_before = 0.052;
		} else if(mode == -135){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = 51;
			v_line = 0.5;
			max_theta = 3.1416 * 2 / 9;
			allround = 3.1416 * 3 / 4;
			keep_after = 0.038;
			keep_before = 0.052;
		} else if(mode == 1350){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = -51;
			v_line = 0.5;
			max_theta = 3.1416 * 2 / 9;
			allround = 3.1416 * 3 / 4;
			keep_after = 0.05;
			keep_before = 0.038;
		} else if(mode == -1350){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = 51;
			v_line = 0.5;
			max_theta = 3.1416 * 2 / 9;
			allround = 3.1416 * 3 / 4;
			keep_after = 0.05;
			keep_before = 0.038;
		} else if(mode == 907){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = -57;
			v_line = 0.5;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2 * 1.0;
			keep_after = 0.03;
			keep_before = 0.03;
		} else if(mode == -907){
			targetLinVelocity = 0.5;
			targetRadVelocity = 0;
			a_theta = 57;
			v_line = 0.5;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2 * 1.0;
			keep_after = 0.03;
			keep_before = 0.03;
		}
	} else if(v == 1.5){
		if(mode == 900){
			targetLinVelocity = 1.5;
			targetRadVelocity = 0;
			a_theta = -85;
			v_line = 1.5;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2;
			keep_after = 0.0;
			keep_before = 0.0;
		} else if(mode == -900){
			targetLinVelocity = 1.5;
			targetRadVelocity = 0;
			a_theta = 85;
			v_line = 1.5;
			max_theta = 3.1416 / 6;
			allround = 3.1416 * 1 / 2;
			keep_after = 0.0;
			keep_before = 0.0;
		}
	}

	float tmpx = x;
    float tmpy = y;
    targetLinVelocity = v_line;
	enable_wall = 0;
	enable_wall_integral = 0;
	integral_rad_gyro = 0;
	enable_gyro = 1;
	enable_gyro_integral = 1;
    Peripherals::setLED('B');
    while (sqrtf((x - tmpx) * (x - tmpx) + (y - tmpy) * (y - tmpy)) <= keep_before) {
        y += v_line * time_i * sinf(3.1416 / 2 - theta);
        x += v_line * time_i * cosf(3.1416 / 2 - theta);
        Timer::wait(1);
    }
    setLinBothendsVelocity(v_line, v_line, keep_before);
    Peripherals::setLED('G');
//	enable_wall = 0;
//	enable_wall_integral = 0;
    while ((int)(1000.0 * sqrtf(absf(max_theta * 2 / a_theta))) > slalomcount) {
        v_theta += a_theta * time_i;
        theta += v_theta * time_i;
		targetRadVelocity = v_theta;
		x += cosf(3.1415 / 2 - theta) * targetLinVelocity * time_i;
		y += sinf(3.1415 / 2 - theta) * targetLinVelocity * time_i;
        ++slalomcount;
        Timer::wait(1);
    }
    slalomcount = 0;
    while (absf(1000.0 * (allround - 2 * max_theta) / absf(v_theta)) > (float)(slalomcount)) {
        theta += v_theta * time_i;
		x += cosf(3.1415 / 2 - theta) * targetLinVelocity * time_i;
		y += sinf(3.1415 / 2 - theta) * targetLinVelocity * time_i;
        ++slalomcount;
        Timer::wait(1);
    }
    slalomcount = 0;
    while ((int)(1000.0 * sqrtf(absf(max_theta * 2 / a_theta))) > slalomcount) {
        v_theta -= a_theta * time_i;
        theta += v_theta * time_i;
		targetRadVelocity = v_theta;
		if (a_theta > 0 && targetRadVelocity < 0) break;
		if (a_theta < 0 && targetRadVelocity > 0) break;
		x += cosf(3.1415 / 2 - theta) * targetLinVelocity * time_i;
		y += sinf(3.1415 / 2 - theta) * targetLinVelocity * time_i;
		++ slalomcount;
		Timer::wait(1);
    }
    tmpx = x;
    tmpy = y;
    targetLinVelocity = v_line;
    Peripherals::setLED('R');
	enable_wall = 1;
	enable_wall_integral = 1;
    while (sqrtf((x - tmpx) * (x - tmpx) + (y - tmpy) * (y - tmpy)) <= keep_after) {
        y += v_line * time_i * sinf(3.1416 / 2 - theta);
        x += v_line * time_i * cosf(3.1416 / 2 - theta);
        Timer::wait(1);
    }
	targetLinVelocity = v;
	targetRadVelocity = 0;
	Peripherals::setLED(' ');
}



