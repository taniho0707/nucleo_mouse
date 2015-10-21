/**
 * @file Algorithm.h
 * 探索アルゴリズムや最短ルート計算などをまとめたファイルです
 */
#ifndef INCLUDED_ALGORITHM_H
#define INCLUDED_ALGORITHM_H

#include "const.h"
#include "Position.h"
#include "Map.h"
#include "Peripherals.h"
#include "Datalog.h"
#include "Sensor.h"


/**
 * 探索アルゴリズムと最短ルート計算をするクラス
 */
class Algorithm{
private:

	void adachiRenewFootmap(Triple<char> input, Footmap &foot, char is_first);

public:

	enum EMouseAngle currentAngle;
	Pair<char> goalCoordinate;
	Pair<char> currentCoordinate;
	Map mapdata;
	Footmap footdata;


	/**
	 * @brief デフォルトコンストラクタ。迷ったらこれ
	 */
	Algorithm();

	/**
	 * @brief ゴール座標を設定可能なコンストラクタ。プチ大会など
	 * @param goal ゴール座標
	 */
	Algorithm(Pair<char> goal);

	/**
	 * @brief 迷路データとゴール座標を設定可能なコンストラクタ。最短走行など
	 * @param goal ゴール座標
	 * @param map 迷路データ
	 */
	Algorithm(Pair<char> goal, Map &map);

	/**
	 * @brief 迷路データとゴール座標と現在座標を設定可能なコンストラクタ。重ね探索など
	 * @param goal ゴール座標
	 * @param map 迷路データ
	 * @param current 現在座標をTriple(tはEMouseAngleで指定)
	 */
	Algorithm(Pair<char> goal, Map &map, Triple<char> current);


	/**
	 * @brief 左手法をするモード
	 * @param wall 渡す壁情報
	 * @param m (左)手法の登録番号
	 * @return 次の進路
	 */
	MouseMotion hidarite(Walldata wall, int m);

	/**
	 * @brief 足立法をするモード
	 * @param wall 渡す壁情報
	 * @param count 現在までの探索回数を指定する(重ね探索かどうかのチェックに使います)
	 * @param mode 動作するモードを選択する<br>
	 * 0->片道<br>
	 * 1->往復<br>
	 * 2->全面<br>
	 * 10->最短
	 * @return 次の進路
	 * @todo モード選択をきちんとする
	 */
	MouseMotion adachi(Walldata wall, int count, int mode);

	/**
	 * @brief 最短パス生成用関数
	 * @param path パスを渡す
	 * @param v 最高速
	 * @param mode モード(0:斜めなし，1:大回り，2:斜め)
	 * @return パス数
	 */
	int getPath(Path &path, float v, int mode);

};


#endif
