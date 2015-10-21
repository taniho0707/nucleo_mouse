/**
 * @file Motor.h
 */
#ifndef INCLUDED_MOTOR_H
#define INCLUDED_MOTOR_H


#include <mathf.h>
#include "iodefine.h"
#include "const.h"
#include "parameters.h"
#include "Encoder.h"
#include "Timer.h"
#include "Sensor.h"
#include "Peripherals.h"
#include "Datalog.h"


/**
 * モーターの制御を担っているクラスです<br>
 * 目標速の算出とその実現を担当しています
 */
class Motor{
private:
	static float param_ideal;

	static int working;

	static float currentLinVelocity;
	static float currentRadVelocity;

	static float idealTargetLinVelocityR;
	static float idealTargetLinVelocityL;
	static float targetVelocityL;
	static float targetVelocityR;
	static float targetLinVelocity;
	static float targetLinAccel;
	static float targetRadVelocity;
	static float targetRadAccel;

	static float targetDistance;

	static int enable_ideal;
	static int enable_wall;
	static int enable_wall_integral;
	static int enable_velocity;
	static int enable_rad;
	static int enable_gyro;
	static int enable_gyro_integral;
	static int enable_distance;
	static int enable_sensor;

	static float integralLin;
	static float integralRad;
	static float integralWall;
	static float integral_lin_encoder;
	static float integral_rad_gyro;
	static float integral_wall;

	static unsigned char isOblique;

	Motor();

public:
	static int dead;
	/// @todo 廃止予定
	static int enable_expect_gyro;


	/**
	 * @brief 絶対値を返す関数
	 * @todo 数学関数にまとめる
	 */
	static int abs(int);
	/**
	 * @brief 絶対値を返す関数
	 * @todo 数学関数にまとめる
	 */
	static float absf(float);


	/**
	 * @brief モーターの目標値追従を開始する
	 */
	static void start();
	/**
	 * @brief モーターの目標値追従を停止する
	 */
	static void stop();

	/**
	 * @brief モーターのデューティー比を設定する
	 * @param side 設定するモーターの番号
	 * @param duty 設定するデューティー比
	 * @tudo 最大速を256として作り直す
	 */
	static void setSpeed(EMotorPosition side, int duty);

	/**
	 * @brief モーターの速度を調整する<br>
	 * この関数を正確に1msごとに呼び出す必要がある
	 */
	static void interrupt();


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
	 * @brief インテグラル項をすべて無効化する
	 */
	static void enable_integral();

	static void disable_integral();


	/**
	 * @brief エンコーダ値の積分値をクリアする
	 */
	static void resetIntegral();

	/**
	 * @brief ジャイロ値の積分値をクリアする
	 */
	static void resetGyroIntegral();

	/**
	 * @brief 斜め制御に切り替える
	 */
	static void changeToOblique();

	static void changeToStraight();

	static unsigned char isCurrentOblique();

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
