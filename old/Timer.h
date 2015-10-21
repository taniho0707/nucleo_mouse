/**
 * @file Timer.h
 * 正確に秒数を計ることを担ったクラス
 */
#ifndef INCLUDED_TIMER_H
#define INCLUDED_TIMER_H

#include "iodefine.h"

/**
 * 正確に秒数を計ることを担ったクラス
 */
class Timer{
private:
	static unsigned long long total;
	static unsigned int waitnum;

	Timer();

public:
	/**
	 * @brief タイマーの1msごと割り込みを開始させる
	 */
	static void start();

	/**
	 * @brief タイマーの割り込みを停止させる
	 */
	static void stop();


	/**
	 * @brief 指定時間待つ。時間経過後に関数から抜ける
	 * @param t 待機する時間(ms)
	 */
	static void wait(int t);

	/**
	 * @brief タイマーの加算を行う。<br>
	 * 正確に1msごとに呼ばれる必要がある。
	 */
	static void interrupt();


};



#endif
