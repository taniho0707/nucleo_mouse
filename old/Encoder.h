/**
 * @file Encoder.h
 */
#ifndef INCLUDED_ENCODER_H
#define INCLUDED_ENCODER_H

#include "iodefine.h"
#include "const.h"
#include "parameters.h"


/**
 * エンコーダの状態を管理するクラスです
 */
class Encoder{
private:

	/**
	 * @brief エンコーダ1台分の情報を格納する構造体です
	 * @todo 多分使ってないメンバがある
	 */
	struct stateEncoder{
		long count;
		float velocity;
	};

	static stateEncoder enc_r;
	static stateEncoder enc_l;

	Encoder();

public:

	/**
	 * @brief エンコーダの計測を開始します
	 */
	static void start();
	/**
	 * @brief エンコーダの計測を停止します
	 */
	static void stop();


	/**
	 * @brief エンコーダの値を取得します<br>
	 * この関数は正確に1msごとに呼ばれる必要があります。
	 */
	static void interrupt();


	/**
	 * @brief エンコーダのカウント数を返します
	 * @param side 取得するエンコーダの番号
	 * @return カウント数が正負で返ってきます
	 */
	static long getCount(EMotorPosition side);

	/**
	 * @brief エンコーダから計算した速度を返します
	 * @param side 取得するエンコーダの番号
	 * @return 速度が正負のfloatで返ってきます
	 */
	static float getVelocity(EMotorPosition side);

};


#endif
