/**
 * @file Datalog.h
 */
#ifndef INCLUDED_DATALOG_H
#define INCLUDED_DATALOG_H

#include "r_flash_api_rx\r_flash_api_rx_if.h"
#include "CExtern.h"
#include "const.h"
#include "Map.h"


/**
 * データフラッシュの操作全般を担うクラスです
 */
class Datalog{
private:

	static unsigned short *mazePos;
	static uint32_t *pos;
	static int datapos;
	static int enabled;
	static int count;

	Datalog();

public:

	/**
	 * @brief データフラッシュの書き込み準備を行います
	 */
	static void setup();

	/**
	 * @brief データフラッシュ領域をすべて0にします
	 */
	static void cleanup();


	static void cleanupWithoutOneMaze(int m);

	/**
	 * @brief データの書き込みを行うことができます
	 */
	static void interrupt();

	/**
	 * @brief ログデータを16バイト分(int型4個)データフラッシュに書き込みます
	 * @param a データ1個目
	 * @param b データ2個目
	 * @param c データ3個目
	 * @param d データ4個目
	 * @return 成功した場合0，失敗した場合1が返る予定です
	 */
	static int saveData(int, int, int, int);

	/**
	 * @brief ログデータを読み込みます
	 * @param n n番目のデータを読み込みます
	 * @param data ログデータがint[4]で返ってきます
	 */
	static void loadData(int n, int *data);

	/**
	 * @brief ログデータを削除します
	 */
	static void clearData();


	/**
	 * @brief interrupt関数を有効化します
	 * @param count データを取得する周期を設定します(ms)
	 */
	static void start(int count);


	/**
	 * @brief データフラッシュに保存してあるパラメータを読み込みます
	 * @params void 要検討
	 * @return 読み込んだパラメータ
	 */
	static int loadBuildinParameter(void);

	/**
	 * @brief データフラッシュにパラメータを保存します
	 * @params void 要検討
	 * @return 成功した場合0，失敗した場合1が返る予定です
	 */
	static int saveBuildParameter(void);

	/**
	 * @brief 迷路データを保存します
	 * @param &map 保存するMapクラスの参照
	 * @param n 保存先の番号(0～3)
	 * @return 成功した場合0，失敗した場合1が返る予定です
	 */
	static int saveMaze(Map &map, int n);

	/**
	 * @brief 迷路データを読み出します
	 * @param &map 読み出すMapクラスの参照
	 * @param n 読み出し先の番号(0～3)
	 * @return 成功した場合0，失敗した場合1が返る予定です
	 */
	static int loadMaze(Map &map, int n);

};


#endif
