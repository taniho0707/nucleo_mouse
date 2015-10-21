/**
 * @file Position.cpp
 */

#include "Position.h"


int Position::x = 0;
int Position::y = 1;
EMouseAngle Position::angle = E_AngleUp;


void Position::setPosition(Pair<char> t, EMouseAngle a){
	x = t.x;
	y = t.y;
	angle = a;
}

void Position::setNextAngle(EMotion motion){

	switch(motion){
	case EMotion_null:

		break;
	case EMotion_Straight:
		if(angle == E_AngleUp){
			++ y;
		} else if(angle == E_AngleRight){
			++ x;
		} else if(angle == E_AngleDown){
			-- y;
		} else if(angle == E_AngleLeft){
			-- x;
		}
		break;
	case EMotion_Right90:
		if(angle == E_AngleUp){
			++ x;
			angle = E_AngleRight;
		} else if(angle == E_AngleRight){
			-- y;
			angle = E_AngleDown;
		} else if(angle == E_AngleDown){
			-- x;
			angle = E_AngleLeft;
		} else if(angle == E_AngleLeft){
			++ y;
			angle = E_AngleUp;
		}
		break;
	case EMotion_Left90:
		if(angle == E_AngleUp){
			-- x;
			angle = E_AngleLeft;
		} else if(angle == E_AngleRight){
			++ y;
			angle = E_AngleUp;
		} else if(angle == E_AngleDown){
			++ x;
			angle = E_AngleRight;
		} else if(angle == E_AngleLeft){
			-- y;
			angle = E_AngleDown;
		}
		break;
	case EMotion_Turn180:
	case EMotion_Turn180_None:
	case EMotion_OnlyTurn180:
		if(angle == E_AngleUp){
			-- y;
			angle = E_AngleDown;
		} else if(angle == E_AngleRight){
			-- x;
			angle = E_AngleLeft;
		} else if(angle == E_AngleDown){
			++ y;
			angle = E_AngleUp;
		} else if(angle == E_AngleLeft){
			++ x;
			angle = E_AngleRight;
		}
		break;
	case EMotion_stop:
		if(angle == E_AngleUp){

		} else if(angle == E_AngleRight){

		} else if(angle == E_AngleDown){

		} else if(angle == E_AngleLeft){

		}
		break;
	default:
		break;
	}
}

Pair<char> Position::getCoordinate(){
	Pair<char> tmp;
	tmp.x = x;
	tmp.y = y;
	return tmp;
}

EMouseAngle Position::getAngle(){
	return angle;
}

EMouseAngle Position::getReverseAngle(EMouseAngle ang){
	if(ang == E_AngleUp) return E_AngleDown;
	else if(ang == E_AngleRight) return E_AngleLeft;
	else if(ang == E_AngleDown) return E_AngleUp;
	else return E_AngleRight;
}



