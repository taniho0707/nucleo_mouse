/**
 * @file Peripherals.h
 */
#ifndef INCLUDED_PERIPHERALS_H
#define INCLUDED_PERIPHERALS_H

#include <stdio.h>
#include <stdarg.h>
#include "iodefine.h"
#include "const.h"
#include "Timer.h"


class Peripherals{
private:

	Peripherals();

public:
	/**
	 * @brief LEDを1色だけ点灯します
	 * @param color 光らせる色を指定<br>
	 * 'R'，'G'，'B'，' 'が使用可能
	 */
	static void setLED(char color);

	/**
	 * @brief LEDを追加で点灯できます
	 * @param color 追加で光らせる色を指定<br>
	 * 'R'，'G'，'B'，' 'が使用可能
	 */
	static void addLED(char color);

	/**
	 * @brief 押しボタンの状態を取得します
	 * @return SW1が0x01，SW2が0x02としてその論理和がintで返ってきます
	 */
	static int getSwitch();

	/**
	 * @brief LEDを点滅させます<br>
	 * 点滅が終了すると関数を抜けます
	 * @param color 光らせる色を指定<br>
	 * 'R'，'G'，'B'，' 'が使用可能
	 * @param t 点滅の周期=2t
	 * @param freq 点滅させる回数を指定。0を指定すると無限に続ける
	 * @todo 無限に点滅を続けないようにしたい
	 */
	static void flickLED(char color, int t, int freq);


	/**
	 * @brief 単音の再生。先に音楽の再生をしていないと音が出ないようになっている。
	 * @param freq 音の周波数
	 * @param t 音を流す時間をmsで指定
	 */
	static void playTone(long freq, int t);

	/**
	 * @brief 音楽の再生
	 * @param n 音楽の番号<br>
	 * 	1	星のカービィ3 起動音<br>
	 * 	2	星のカービィ3 決定音<br>
	 * 	3	星のカービィ64 初心者の部屋 1～4小節目<br>
	 * 	4	星のカービィ64 初心者の部屋 33～36小節目<br>
	 * 	5	星のカービィ64 ポップスター ステージセレクト 1～8小節目<br>
	 * 	6	星のカービィ64 ポップスター ステージセレクト 25～32小節目<br>
	 * 	7	星のカービィ ステージクリア(長い方)<br>
	 * 	8	星のカービィ ステージクリア(短い方)<br>
	 * 	9	星のカービィ3 正解<br>
	 * 	10	星のカービィ 瀕死<br>
	 * 	11	星のカービィ 1up<br>
	 * 	12	星のカービィ 死亡<br>
	 * 	13	星のカービィグルメレース(原曲)<br>
	 *	default	星のカービィ3 カーソル移動音
	 */
	static void playMusic(int n);

	/**
	 * @brief モールス音の長音を再生
	 */
	static void playMorseLong();
	/**
	 * @brief モールス音の長音を再生
	 */
	static void playMorseShort();

	/**
	 * @brief 数字1桁のモールス音を再生
	 * @param n 再生する数字を指定
	 * @todo アルファベットも再生できるように
	 */
	static void playMorse(char n);

	/**
	 * @brief 複数桁の数字のモールス音を再生
	 * @param n 再生する数列を指定
	 * @param count 再生する文字数
	 * @todo アルファベットも再生できるように
	 */
	static void playMorses(char* n, int count);


	/**
	 * @brief 1バイト文字をシリアル送信します。常用できません。
	 * @param c 送信する文字
	 */
	static void put1byte(char c);

	/**
	 * @brief 複数バイトの文字をシリアル送信します。常用できません。
	 * @param c 送信する文字列
	 * @param n 送信する文字数
	 */
	static void putnbyte(char *c, int n);

	/**
	 * @brief 混合文字列をシリアル送信します<br>
	 * 条件付き書式で指定してください
	 */
	static int myprintf(const char *, ...);

};



#endif
