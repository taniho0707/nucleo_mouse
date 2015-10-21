/**
 * @file Map.h
 * 迷路データを扱うためのクラスや，それに付随したいくつかのクラスを用意しています。<br>
 * 二次元，三次元ベクトルや壁データのクラスがあります。
 */
#ifndef INCLUDED_MAP_H
#define INCLUDED_MAP_H

#include "const.h"

/**
 * @brief 壁データを簡単に扱うためのクラス。<br>
 * MapクラスやSensorクラスとの壁情報のやりとりに用いる方が良い。
 */
class Walldata{
private:
	char data;

public:
	/**
	 * @brief コンストラクタ<br>初期値は壁なし<br>右ビットから順に前，左，右，後のデータが格納されている
	 */
	Walldata(){
		data = 0;	//0b0000XXXX
	}

	/**
	 * @brief =演算子のオーバーロード<br>char型の代入ができます。
	 * @param input 代入したい変数
	 */
	void operator= (char input){
		data = input;
	}

	void operator= (Walldata input){
		data = input.data;
	}

	void operator+= (char input){
		data |= input;
	}

	void operator|= (char input){
		data |= input;
	}

	/**
	 * @brief クラスが保持している内部データを返します。
	 * @return クラスが保持している内部データ
	 */
	char getRawData(){
		return data;
	}

	/**
	 * @brief 指定した方向に壁があるか返します。
	 * @param dir 壁を調べたい方向
	 * @return 壁があれば1，なければ0をintで返します
	 */
	int existWall(EMouseDirection dir){
		if((data>>dir)&1) return 1;
		else return 0;
	}

	/**
	 * @brief 指定した方向に壁を追加します。
	 * @param dir 壁を追加したい方向
	 */
	void addWall(EMouseDirection dir){
/*		char tmp = 1;
		tmp << dir;
		data |= tmp;
*/
		if(dir == E_DirFront)
			data |= 0x01;
		else if(dir == E_DirLeft)
			data |= 0x02;
		else if(dir == E_DirRight)
			data |= 0x04;
		else if(dir == E_DirBack)
			data |= 0x08;
		return;
	}

	/**
	 * @brief 指定した方向の壁を除去します。
	 * @param angle 壁を除去したい方向
	 */
	void removeWall(EMouseDirection angle){
		char tmp = 1;
		tmp << angle;
		tmp = ~tmp;
		data &= tmp;
		return;
	}

	~Walldata(){

	}
};

/**
 * 二次元ベクトルを実装したクラス
 */
template <typename T>
class Pair{
private:

public:
	T x;
	T y;

	/**
	 * @brief コンストラクタ<br>ゼロベクトルとして初期化します
	 */
	Pair<T>(){
		x = 0;
		y = 0;
	}
	Pair<T>(T _x, T _y){
		x = _x;
		y = _y;
	}

	Pair<T>& operator= (Pair<T> tmp){
		x = tmp.x;
		y = tmp.y;
		return *this;
	}

	Pair<T> operator+ (Pair<T> input){
		Pair tmp;
		tmp.x = x + input.x;
		tmp.y = y + input.y;
		return tmp;
	}

	T operator== (Pair<T> input){
		if(x == input.x && y == input.y) return 1;
		else return 0;
	}

	~Pair<T>(){

	}
};

/**
 * 三次元ベクトルを実装したクラス
 */
template <typename T>
class Triple{
private:

public:
	T x;
	T y;
	T t;

	/**
	 * @brief コンストラクタ<br>ゼロベクトルとして初期化します
	 */
	Triple<T>(){
		x = 0;
		y = 0;
		t = 0;
	}
	Triple<T>(T _x, T _y, T _t){
		x = _x;
		y = _y;
		t = _t;
	}

	Triple<T>& operator= (Triple<T> tmp){
		x = tmp.x;
		y = tmp.y;
		t = tmp.t;
		return *this;
	}

	Triple<T> operator+ (Triple<T> input){
		Triple<T> tmp;
		tmp.x = x + input.x;
		tmp.y = y + input.y;
		tmp.t = t + input.t;
		return tmp;
	}

	int operator== (Triple<T> input){
		if(x == input.x && y == input.y && t == input.t) return 1;
		else return 0;
	}

	~Triple<T>(){

	}
};

/**
 * Tripleクラスでのキューを実装したクラス<br>
 * バッファーサイズはQUEUE_BUF_SIZE<br>
 * @todo リングバッファの実装
 */
template <typename T>
class Queue{
private:
	Triple<T> data[QUEUE_BUF_SIZE];
	unsigned short currentTop;
	unsigned short currentEnd;

public:
	/**
	 * @brief コンストラクタ<br>
	 * 三次元バッファの初期化を行う
	 */
	Queue<T>(){
		currentTop = 0;
		currentEnd = 0;
		for(unsigned short i=0; i<QUEUE_BUF_SIZE; i++){
			data[i].x = 0;
			data[i].y = 0;
			data[i].t = 0;
		}
	}

	/**
	 * @brief プッシュする
	 * @param tmp プッシュするTripleデータ
	 * @return 成功すると0，失敗すると1をintで返す
	 */
	int push(Triple<T> tmp){
		if(currentEnd > QUEUE_BUF_SIZE) return 1;
		data[currentEnd] = tmp;
		++ currentEnd;
		return 0;
	}

	/**
	 * @brief ポップする<br>
	 * この関数の前に必ずempty()でデータが存在するかを確認する必要がある
	 * @return 次のTripleデータを返す
	 */
	Triple<T> pop(){
		++ currentTop;
		return data[currentTop - 1];
	}

	/**
	 * @brief データが存在するか確認する
	 * @return データが存在したら0，存在しなかったら1を返す
	 */
	int empty(){
		if(currentTop == currentEnd) return 1;
		else return 0;
	}

	/**
	 * @brief キューをリセットし，ポインタを先頭に戻す
	 */
	void reset(){
		currentTop = 0;
		currentEnd = 0;
	}

	~Queue<T>(){

	}
};


/**
 * 歩数マップを管理するクラス
 */
class Footmap{
private:

public:
	char map[16][16];

	Footmap(){
		for(int i=0; i<16; ++i){
			for(int j=0; j<16; ++j) map[i][j] = 255;
		}
	}

	/**
	 * @brief 歩数マップをクリアします
	 */
	void resetFootmap(){
		for(int i=0; i<16; ++i){
			for(int j=0; j<16; ++j) map[i][j] = 255;
		}
	}

	/**
	 * @brief 歩数マップを取得します
	 * @param x 取得するx座標
	 * @param y 取得するy座標
	 * @param out 座標外の返り値
	 * @return 設定した座標の歩数
	 */
	int getFootmap(int x, int y, int out){
		if(x < 0 || x > 15 || y < 0 || y > 15) return out;
		else return map[x][y];
	}

	/**
	 * @brief 歩数マップを設定します
	 * @param x 設定するx座標
	 * @param y 設定するy座標
	 * @param data 設定する歩数
	 */
	void setFootmap(int x, int y, char data){
		if(x < 0 || x > 15 || y < 0 || y > 15) return;
		if(data > 255) map[x][y] = 255;
		else map[x][y] = data;
		return;
	}

	~Footmap(){

	}
};


/**
 * パスを管理するクラス
 */
class Path{
private:

	unsigned char currentPos;

public:

	Path(){
		currentPos = 0;
		for(int i=0; i<256; ++i){
			data[i] = 0;
			division[i] = 0;
		}
	}

//	MouseMotion data[128];
	unsigned char data[256];
	unsigned char division[256];


	MouseMotion getNext(){
		MouseMotion next_motion = {
				EMotion_null,
				0,
				0.5,
				EMotion_null
		};
		if(currentPos > 255){
			return next_motion;
		}
		next_motion.division = division[currentPos];
		unsigned char next_motion_type = data[currentPos];
//		next_motion.currentMotion = data[currentPos];
		if(next_motion_type == EMotion_Straight){
			next_motion.currentMotion = EMotion_Straight;
		} else if(next_motion_type == EMotion_null){
			next_motion.currentMotion = EMotion_null;
		} else if(next_motion_type == EMotion_Right90){
			next_motion.currentMotion = EMotion_Right90;
		} else if(next_motion_type == EMotion_Left90){
			next_motion.currentMotion = EMotion_Left90;
		} else if(next_motion_type == EMotion_Turn180){
			next_motion.currentMotion = EMotion_Turn180;
		} else if(next_motion_type == EMotion_BigRight90){
			next_motion.currentMotion = EMotion_BigRight90;
		} else if(next_motion_type == EMotion_BigLeft90){
			next_motion.currentMotion = EMotion_BigLeft90;
		} else if(next_motion_type == EMotion_BigRight180){
			next_motion.currentMotion = EMotion_BigRight180;
		} else if(next_motion_type == EMotion_BigLeft180){
			next_motion.currentMotion = EMotion_BigLeft180;
		} else if(next_motion_type == EMotion_Right45i){
			next_motion.currentMotion = EMotion_Right45i;
		} else if(next_motion_type == EMotion_Left45i){
			next_motion.currentMotion = EMotion_Left45i;
		} else if(next_motion_type == EMotion_Right135i){
			next_motion.currentMotion = EMotion_Right135i;
		} else if(next_motion_type == EMotion_Left135i){
			next_motion.currentMotion = EMotion_Left135i;
		} else if(next_motion_type == EMotion_Right45o){
			next_motion.currentMotion = EMotion_Right45o;
		} else if(next_motion_type == EMotion_Left45o){
			next_motion.currentMotion = EMotion_Left45o;
		} else if(next_motion_type == EMotion_Right135o){
			next_motion.currentMotion = EMotion_Right135o;
		} else if(next_motion_type == EMotion_Left135o){
			next_motion.currentMotion = EMotion_Left135o;
		} else if(next_motion_type == EMotion_ObliqueRight90){
			next_motion.currentMotion = EMotion_ObliqueRight90;
		} else if(next_motion_type == EMotion_ObliqueLeft90){
			next_motion.currentMotion = EMotion_ObliqueLeft90;
		} else if(next_motion_type == EMotion_Oblique){
			next_motion.currentMotion = EMotion_Oblique;
		} else {
			next_motion.currentMotion = EMotion_null;
		}
		++ currentPos;
		return next_motion;
		/// @todo 次の動きを用意する
//		return data[currentPos-1];
	}

	/// @todo パスを綺麗に生成する
	void setNext(EMotion next){
		if(currentPos > 255) return;
		data[currentPos] = next;
		++ currentPos;
		return;
	}

	void resetPos(){
		currentPos = 0;
	}

	~Path(){

	}
};


/**
 * 迷路データを管理するクラス
 */
class Map{
private:


public:
	Map();

	/**
	 * @brief 一番左の壁がMSB，右から2番目の壁がLSB，下から順
	 */
	unsigned short column[16];
	/**
	 * @brief 一番左の壁がMSB，右から2番目の壁がLSB，下から順
	 */
	unsigned short row[15];
	/**
	 * @brief 一番左のマスがMSB，一番右のマスがLSB，下から順
	 */
	unsigned short reached[16];


	/**
	 * @brief 迷路データを初期化する
	 */
	void resetMap();
	/**
	 * @brief 到達マップを初期化する
	 */
	void resetReachedMap();


	/**
	 * @brief 壁を追加します
	 * @param x 壁を追加する区画のx座標
	 * @param y 壁を追加する区画のy座標
	 * @param angle 今自分が向いている絶対方向
	 * @param wall 今見えている壁情報
	 */
	void addWall(int x, int y, EMouseAngle angle, Walldata wall);

	/**
	 * @brief 壁を設定します
	 * @param x 壁を追加する区画のx座標
	 * @param y 壁を追加する区画のy座標
	 * @param angle 今自分が向いている絶対方向
	 * @param wall 今見えている壁情報
	 */
	void setWall(int x, int y, EMouseAngle angle, Walldata wall);

	/**
	 * @brief 壁を追加します。絶対方向でのみ指定が可能です。
	 * @param x 壁を追加する区画のx座標
	 * @param y 壁を追加する区画のy座標
	 * @param angle 今自分が向いている絶対方向
	 */
	void addSingleWall(int x, int y, EMouseAngle angle);

	/**
	 * @brief 壁を設定します。絶対方向でのみ指定が可能です。
	 * @param x 壁を追加する区画のx座標
	 * @param y 壁を追加する区画のy座標
	 * @param angle 今自分が向いている絶対方向
	 * @param wall 今見えている壁情報
	 * @bug まだaddSingleWallと同じ関数
	 */
	void setSingleWall(int x, int y, EMouseAngle angle, int wall);


	/**
	 * @brief 絶対方向から見て壁があるか確認します
	 * @param x 壁を追加する区画のx座標
	 * @param y 壁を追加する区画のy座標
	 * @param ang 今自分が見ている絶対方向
	 * @return 壁が存在したら1，なかったら0をintで返します
	 */
	int isExistWall(int x, int y, EMouseAngle ang);


	/**
	 * @brief 到達マップを設定します
	 * @param x 到達設定する区画のx座標
	 * @param y 到達設定する区画のy座標
	 */
	void setReached(int, int);

	/**
	 * @brief 到達したか確認します
	 * @param x 到達確認する区画のx座標
	 * @param y 到達確認する区画のy座標
	 * @return 到達していたら1，していなかったら0を返します
	 */
	int hasReached(int, int);


	/**
	 * @brief マウスから見たWalldataを絶対方向に変換します
	 * @param wall 変換元の壁情報
	 * @param ang マウスの向いている方向
	 * @return 変換後のWalldataを返します
	 */
	Walldata rotateWallToAbsolute(Walldata wall, EMouseAngle ang);

	/**
	 * @brief 絶対方向のWalldataをマウスから見た方向に変換します
	 * @param wall 変換元の壁情報
	 * @param ang 変換したい方向
	 * @return 変換後のWalldataを返します
	 */
	Walldata rotateWallToRelative(Walldata wall, EMouseAngle ang);


	/**
	 * @brief =演算子のオーバーロード。Mapクラスを代入します
	 * @bug できない
	 */
	Map& operator= (Map tmp){
//		column = tmp.column;
//		row = tmp.row;
//		reached = tmp.reached;
		return *this;
	}


	~Map();
};


#endif
