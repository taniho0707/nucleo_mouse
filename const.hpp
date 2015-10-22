/**
 * @file const.hpp
 * 各ソースコード中などで使用する定数宣言をしています。
 */

#ifndef INCLUDED_CONST_HPP
#define INCLUDED_CONST_HPP


#define QUEUE_BUF_SIZE 256


/**
 * @brief スタート地点から前を向いている方向を上として，今自分が向いている絶対的な方向を指定するときに使用します
 */
enum EMouseAngle{
	E_AngleUp,		//0
	E_AngleRight,	//1
	E_AngleDown,	//2
	E_AngleLeft		//3
};

/**
 * @brief アルゴリズムや迷路データ操作中に，今自分が向いている相対的な方向を指定するときに使用します
 */
enum EMouseDirection{
	E_DirFront,	//0
	E_DirLeft,	//1
	E_DirRight,	//2
	E_DirBack	//3
};

/**
 * @brief マウスについている光センサの番号を定義しています
 */
enum ESensorPosition{
	E_LFront,
	E_Left,
	E_Right,
	E_RFront
};

/**
 * @brief モーターの左右の番号を定義しています
 */
enum EMotorPosition{
	E_MotorLeft,	//0
	E_MotorRight	//1
};


/**
 * @brief マウスの進む方向を定義しています<br>
 * あくまで向き角度のみ指定しているので，速度や進む距離などは別途パラメータを用意してください
 */
enum EMotion{
	EMotion_null,			//0
	EMotion_Straight,		//1
	EMotion_Right90,		//2
	EMotion_Left90,			//3
	EMotion_Turn180,		//4
	EMotion_BigRight90,		//5
	EMotion_BigLeft90,		//6
	EMotion_BigRight180,	//7
	EMotion_BigLeft180,		//8
	EMotion_Right45i,		//9
	EMotion_Left45i,		//10
	EMotion_Right135i,		//11
	EMotion_Left135i,		//12
	EMotion_Oblique,		//13
	EMotion_Right45o,		//14
	EMotion_Left45o,		//15
	EMotion_Right135o,		//16
	EMotion_Left135o,		//17
	EMotion_ObliqueRight90,	//18
	EMotion_ObliqueLeft90,	//19
	EMotion_OnlyTurn180,	//20
	EMotion_Turn180_None,	//21
	EMotion_Start,			//22
	EMotion_stop			//23
};

/**
 * @brief マウスの動きを定義しています
 */
struct MouseMotion{
	EMotion currentMotion;
	unsigned division;
	float speed;
	EMotion nextMotion;
};

/**
 * @brief パラメータ群を定義しています
 */
struct SpeedParameter{
	float straight;
	float oblique;
	float turn90;
	float turn180;
	float turn45;
	float turn135;
	float oblique90;
	float normal;
};



#endif /* CONST_H_ */
