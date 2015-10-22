/**
 * @file Usart.hpp
 */
#ifndef INCLUDED_USART_HPP
#define INCLUDED_USART_HPP

#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
/* #include "const.h" */
/* #include "Timer.h" */


class Usart{
private:

	Usart();

public:

	static void initialize();
	/**
	 * @brief 1バイト文字をシリアル送信します。常用できません。
	 * @param c 送信する文字
	 */
	static void send1byte(USART_TypeDef *USARTx, char data);

	/**
	 * @brief 複数バイトの文字をシリアル送信します。常用できません。
	 * @param c 送信する文字列
	 * @param n 送信する文字数
	 */
	static void sendnbyte(USART_TypeDef *USARTx, char *c, int n);

	/**
	 * @brief 混合文字列をシリアル送信します<br>
	 * 条件付き書式で指定してください
	 */
	static void printf(const char *, ...);

};



#endif
