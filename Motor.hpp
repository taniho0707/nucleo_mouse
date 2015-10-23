/**
 * @file Motor.hpp
 */
#ifndef INCLUDED_MOTOR_HPP
#define INCLUDED_MOTOR_HPP


// #include <mathf.h>
#include "const.hpp"
#include "parameters.hpp"
// #include "Encoder.h"
#include "Timer.hpp"
#include "Usart.hpp"
// #include "Sensor.h"
// #include "Peripherals.h"
// #include "Datalog.h"


/**
 * モーターの制御を担っているクラスです<br>
 * 目標速の算出とその実現を担当しています
 */
class Motor{
private:
	static TIM_TimeBaseInitTypeDef TIM_initstr;
	static NVIC_InitTypeDef NVIC_initstr;
	static TIM_OCInitTypeDef TIM_OC_initstr;
	static GPIO_InitTypeDef GPIO_initstr;

	static float target_velocity[2];
	static int count_of_clear[2];
	static float target_accel_gravity;
	static float target_velocity_gravity;

	static float total_distance[2];

	static char is_current_forward[2];

	Motor();

public:

	static void excitate();

	static void disexcitate();

	static void initialize();

	static void startRotate();

	static void stopRotate();

	static void changeToForward(EMotorPosition side);

	static void changeToBackward(EMotorPosition side);

	/**
	 * @brief モーターの速度を変更(予約)する
	 * @param side 設定するモーターの番号
	 * @param velocity 設定する速度
	 */
	static void setSpeed(EMotorPosition side, float velocity);

	/**
	 * @brief モーターの速度を調整する<br>
	 * この関数はPWMのカウンタリセットがかかるたびに左右ずつ呼ばれる必要がある。
	 */
	static int interrupt(EMotorPosition side);


	/**
	 * @brief モーターの目標直線加速度を設定する
	 * @param a 目標直線加速度
	 */
	static void setLinAccel(float a);

	/**
	 * @brief モーターの目標直線速度を設定する
	 * @param a 目標直線速度
	 */
	static void setLinVelocity(float a);


	/**
	 * @brief モーターの目標角加速度を設定する
	 * @param a 目標角加速度
	 */
	static void setRadAccel(float a);

	/**
	 * @brief モーターの目標角速度を設定する
	 * @param a 目標角速度
	 */
	static void setRadVelocity(float a);


	/**
	 * @brief 斜め制御に切り替える
	 */
	static void changeToOblique();

	static void changeToStraight();

	static unsigned char isCurrentOblique();

	static void resetTotalDistance();

	static float getTotalDistanceGravity();

	static float getTotalDistanceAngle();

	static float addTotalDistance(float t, EMotorPosition side);


	static void pulseRun(float max_vel, float distance);

	static void pulseTurn(float max_vel, float degree);

	/**
	 * @brief 加速をする<br>
	 * 加速する場合は目標速まで加速すると，関数から抜ける<br>
	 * 減速や等速の場合は目標距離になると関数から抜ける
	 * @param startvl 初期速度
	 * @param endvl 終端速度
	 * @param distance 進む距離(加速時は無視される)
	 * @return 進んだ距離
	 * @todo もっと使いやすくする
	 */
	static float setLinBothendsVelocity(float startvl, float endvl, float distance);

	/**
	 * @brief 回転加速をする<br>
	 * 加速する場合は目標速まで加速すると，関数から抜ける<br>
	 * 減速や等速の場合は目標距離になると関数から抜ける
	 * @param startvl 初期速度
	 * @param endvl 終端速度
	 * @param digree 回転するラジアン(加速時は無視される)
	 * @return 回転したラジアン
	 * @todo もっと使いやすくする。ジャイロの処理がすこしおかしいかも
	 */
	static float setRadBothendsVelocity(float startvl, float endvl, float degree);


	/**
	 * @brief 台形加速をする
	 * @param distance 進む距離
	 * @param maxSpeed 最高速度
	 * @todo 三角加速に対応
	 */
	static void linTrapezoid(float distance, float startvl, float endvl, float maxSpeed);

	/**
	 * @brief 超信地旋回をする
	 * @param degree 回転するラジアン
	 */
	static void pivotturn(float degree);


	/**
	 * @brief スラロームをする
	 * @param num 実行するスラロームの番号(要加筆)
	 * @param v 最高重心速度
	 * @todo 速度や角度のパラメータを整える
	 */
	static void slalom(int num, float v);


};


#endif
