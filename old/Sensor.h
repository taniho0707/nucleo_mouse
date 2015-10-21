/**
 * @file Sensor.h
 * 光センサ，ジャイロセンサの入力と管理をしているクラス
 */
#ifndef INCLUDED_SENSOR_H
#define INCLUDED_SENSOR_H

#include "iodefine.h"
#include "const.h"
#include "parameters.h"
#include "Map.h"
#include "Timer.h"
#include "Motor.h"

/**
 * 光センサ，ジャイロセンサの入力と管理をしているクラス
 */
class Sensor{
private:

	static float battery;
	static int fl[5];
	static int l[5];
	static int r[5];
	static int fr[5];
	static int currentSensorPos;

	static long gyro;
	static float zeroGyro;
	static float integralGyro;
	static float sensitivGyro_r;
	static float sensitivGyro_l;

	static int ref_fl;
	static int ref_l;
	static int ref_r;
	static int ref_fr;
	static int lim[4];
	static int ref_obl_l;
	static int ref_obl_r;

	static int working;

	Sensor();

	/**
	 * @brief int配列の平均値を返します
	 * @param array 平均値を出したい配列へのポインタ
	 * max 配列の要素数
	 * @return 配列の平均値をintで返します
	 */
	static int getAvgArray(int *, int);

	/**
	 * @brief 任意の整数の絶対値を返します
	 * @param i 絶対値を出したい整数
	 * @return 整数値をintで返します
	 * @todo 数学関数を別にまとめる
	 */
	static int removeNegative(int);

public:

	/**
	 * @brief 電源電圧を返します
	 * @return 電圧をfloatで返します
	 */
	static float getBattery();

	/**
	 * @brief ジャイロのゼロ点を合わせます。<br>
	 * 所要時間は1秒。
	 */
	static void resetGyro();

	/**
	 * @brief ジャイロ値を変換した角度を返します
	 * @return 現在の角速度をfloatで返します<br>
	 * 右回転が正，左回転が負となります
	 */
	static float getGyroRad();

	/**
	 * @brief 基準用の3.3Vの生データを返します
	 * @return 3.3Vの生データをlongで返します
	 */
	static long get3V3();


	/**
	 * @brief センサーの初期設定を行い，センサー値の取得をはじめます
	 */
	static void start();

	/**
	 * @brief センサー値の取得を停止します<br>
	 * またセンサー値を0に初期化します
	 */
	static void stop();

	/**
	 * @brief センサー値の取得及び保存加工処理を行います<br>
	 * 正確に1msごとに呼び出される必要があります
	 */
	static void interrupt();


	/**
	 * @brief 過去5回分のセンサ値の平均を返します
	 * @param t 取得するセンサーを指定してください
	 * @return センサ値の平均をintで返します
	 * @todo 平均値と生の値のどちらを返すか選択可能にしたい
	 */
	static int getValue(ESensorPosition);

	/**
	 * @brief 任意のセンサーが壁を検知しているか返します
	 * @param t 取得するセンサーを指定してください
	 * @return 壁があれば1，なければ0をintで返します
	 */
	static int existWall(ESensorPosition);

	/**
	 * @brief 前と左右の壁状況を返します<br>
	 * 壁情報の取得には，この関数を使ってください
	 * @return 壁の存在をWalldataで返します
	 */
	static Walldata getWall();

	/**
	 * @brief 左右の壁との距離から，現在の位置が中央線よりどれだけずれているかを返します
	 * @return 右にずれていれば正，左にずれていれば負の整数がintとして返します
	 * @todo もう少し良いアルゴリズムがあるはず
	 */
	static int getCorrection(int max);

	/**
	 * @brief 目標前壁位置から現在がどれだけずれているかを返しています
	 * @return 手前なら正，行き過ぎなら負の整数がintとして返します
	 */
	static int getFrontCorrection();

	/**
	 * @brief ジャイロゼロ点を出力します
	 * @return ジャイロゼロ点電圧
	 */
	static float getGyroZero();

	/**
	 * @brief ジャイロ積分値を出力します
	 * @return ジャイロ積分値
	 */
	static float getGyroIntegral();

};



#endif
