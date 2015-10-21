/**
 * @file Control.h
 */
#ifndef INCLUDED_CONTROL_H
#define INCLUDED_CONTROL_H

#include "const.h"
#include "Timer.h"
#include "Peripherals.h"
#include "Encoder.h"
#include "Sensor.h"
#include "Motor.h"
#include "Datalog.h"
#include "Map.h"
#include "Algorithm.h"
#include "Mouse.h"


#define NUM_MODE 6
#define NUM_MODE0 5
#define NUM_MODE1 5
#define NUM_MODE2 2
#define NUM_MODE3 5
#define NUM_MODE4 11
#define NUM_MODE5 5


extern Map mapdata;


class Control{
private:

	static int mode, submode;
	static int isFirst;

	static int x, y;

	static Pair<char> goal;

	Control();

public:

	/**
	 * @brief はじめにこの関数を呼びましょう。
	 */
	static void modeselect();

	/**
	 * @brief 試験用の関数です。<br>
	 * modeselect()の代わりに呼び出して実験を行うことができます。
	 */
	static void test();

	/**
	 * @brief センサ値のチェックをするモード
	 */
	static void sensorcheck();

	/**
	 * @brief ログデータを出力する
	 */
	static void showLogdata();

	/**
	 * @brief 迷路データを出力する
	 * @param n 出力する迷路データの番号
	 */
	static void showMazedata(int n);

	/**
	 * @brief パスデータを出力する
	 * @param m パス生成する迷路データ
	 */
	static void showPathdata(int m);

	/**
	 * @brief スラロームの調整をするモード
	 * @param m スラロームの登録番号
	 * @param v 最高重心速度
	 */
	static void slalommode(int m, float v);

	/**
	 * @brief 5区画直進モード
	 * @params v 速度
	 */
	static void straightmode(float v);

	/**
	 * @brief ジャイロの調整をするモード
	 * @param m 調整の登録番号
	 */
	static void spinmode(int m);

	/**
	 * @brief 左手法をするモード
	 * @param m (左)手法の登録番号
	 */
	static void hidarite(int loadmaze, int savemaze, int m);

	/**
	 * @brief 足立法をするモード
	 * @param loadmaze 規定外なら迷路をロードしません。
	 * @param savemaze
	 * @param mode 動作するモードを選択する<br>
	 * 0->片道<br>
	 * 1->往復<br>
	 * 2->全面
	 */
	static void adachi(int loadmaze, int savemaze, int mode);

	/**
	 * @brief 最短モード
	 * @param v 最高速を指定
	 * @param mode1 予約
	 * @param mode2 予約
	 */
	static void firstrun(float lin_v, float turn_v, float obl_v, int loadmaze, int mode);

	/**
	 * @brief サーキットモード
	 * @param v 最高速を指定
	 * @param size 迷路サイズ<br>0 -> フルサイズ<br>1 -> ハーフサイズ
	 */
	static void circuit(float strait_v, float turn_v, float obl_v, int size);

};


#endif
