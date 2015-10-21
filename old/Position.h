#ifndef INCLUDED_POSITION_H_
#define INCLUDED_POSITION_H_
/**
 * @file Position.h
 */

#include "Map.h"
#include "const.h"


/**
 * @brief マウスの現在座標と向きを管理するクラスです。
 */
class Position{
private:


	Position();

public:

	static int x;
	static int y;

	static EMouseAngle angle;

	static void setPosition(Pair<char>, EMouseAngle);
	static void setNextAngle(EMotion);
	static Pair<char> getCoordinate();
	static EMouseAngle getAngle();
	static EMouseAngle getReverseAngle(EMouseAngle);

};

#endif
