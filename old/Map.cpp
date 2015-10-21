/**
 * @file Map.cpp
 */

#include "Map.h"


Map::Map(){
	resetMap();
	resetReachedMap();
	column[0] |= 32768;
}

Map::~Map(){
}

void Map::resetMap(){
	for (int i = 0; i < 16; i++) {
		column[i] = 0;
		if(i < 15) row[i] = 0;
	}
}

void Map::resetReachedMap(){
	for (int i = 0; i < 16; i++) {
		reached[i] = 0;
	}
}

void Map::addWall(int x, int y, EMouseAngle angle, Walldata wall){
	Walldata tmp = rotateWallToAbsolute(wall, angle);
	if(tmp.existWall(E_DirFront)) addSingleWall(x, y, E_AngleUp);
	if(tmp.existWall(E_DirRight)) addSingleWall(x, y, E_AngleRight);
	if(tmp.existWall(E_DirBack)) addSingleWall(x, y, E_AngleDown);
	if(tmp.existWall(E_DirLeft)) addSingleWall(x, y, E_AngleLeft);
	return;
}

void Map::setWall(int x, int y, EMouseAngle angle, Walldata wall){
	Walldata tmp = rotateWallToAbsolute(wall, angle);
	setSingleWall(x, y, E_AngleUp, tmp.existWall(E_DirFront));
	setSingleWall(x, y, E_AngleRight, tmp.existWall(E_DirRight));
	setSingleWall(x, y, E_AngleDown, tmp.existWall(E_DirBack));
	setSingleWall(x, y, E_AngleLeft, tmp.existWall(E_DirLeft));
	return;
}

void Map::addSingleWall(int x, int y, EMouseAngle angle){
	if((x == 0 && angle == E_AngleLeft)
			|| (x == 15 && angle == E_AngleRight)
			|| (y == 0 && angle == E_AngleDown)
			|| (y == 15 && angle == E_AngleUp)) return;
	if(angle == E_AngleUp){
		row[y] |= (32768 >> x);
	} else if(angle == E_AngleRight){
		column[y] |= (32768 >> x);
	} else if(angle == E_AngleDown){
		row[y-1] |= (32768 >> x);
	} else if(angle == E_AngleLeft){
		column[y] |= (32768 >> (x-1));
	}
}

void Map::setSingleWall(int x, int y, EMouseAngle angle, int wall){
	if((x == 0 && angle == E_AngleLeft)
			|| (x == 15 && angle == E_AngleRight)
			|| (y == 0 && angle == E_AngleDown)
			|| (y == 15 && angle == E_AngleUp)) return;
	int tmpx = (angle == 3) ? x-1 : x;
	int tmpy = (angle == 2) ? y-1 : y;
	if(angle % 2){
		row[tmpy] |= (32768 >> tmpx);
	} else {
		column[tmpy] |= (32768 >> tmpx);
	}
}


int Map::isExistWall(int x, int y, EMouseAngle ang){
	int ans = 0;
	if((x == 0 && ang == E_AngleLeft)
			|| (x == 15 && ang == E_AngleRight)
			|| (y == 0 && ang == E_AngleDown)
			|| (y == 15 && ang == E_AngleUp)) return 1;
	if(ang == E_AngleUp){
		ans = row[y] & (32768 >> x);
	} else if(ang == E_AngleRight) {
		ans = column[y] & (32768 >> x);
	} else if(ang == E_AngleDown){
		ans = row[y-1] & (32768 >> x);
	} else if(ang == E_AngleLeft){
		ans = column[y] & (32768 >> (x-1));
	} else {
		ans = 1;
	}
	if(ans) return 1;
	else return 0;
}

void Map::setReached(int x, int y){
	if(x > 15 || x < 0 || y > 15 || y < 0) return;
	reached[y] |= (32768 >> x);
	return;
}

int Map::hasReached(int x, int y){
	if(reached[y] & (32768 >> x)) return 1;
	else if(
			(reached[y] & (32768 >> (x+1)))
			&& (reached[y] & (32768 >> ((x==0)?(x+1):(x-1))))
			&& (reached[(y==15)?(y-1):(y+1)] & (32768 >> x))
			&& (reached[(y==0)?(y+1):(y-1)] & (32768 >> x))
			) return 1;
	else return 0;
}

Walldata Map::rotateWallToAbsolute(Walldata wall, EMouseAngle ang){
	Walldata tmp;
/*	char indata[4];
	indata[0] = wall.existWall(E_DirFront);
	indata[1] = wall.existWall(E_DirRight);
	indata[2] = wall.existWall(E_DirBack);
	indata[3] = wall.existWall(E_DirLeft);
	if(indata[(4-ang)%4]) tmp.addWall(E_DirFront);
	if(indata[(5-ang)%4]) tmp.addWall(E_DirRight);
	if(indata[(6-ang)%4]) tmp.addWall(E_DirBack);
	if(indata[(7-ang)%4]) tmp.addWall(E_DirLeft);
*/
	if(ang == E_AngleUp) return wall;
	else if(ang == E_AngleRight){
		if(wall.existWall(E_DirFront)) tmp.addWall(E_DirRight);
		if(wall.existWall(E_DirRight)) tmp.addWall(E_DirBack);
		if(wall.existWall(E_DirBack)) tmp.addWall(E_DirLeft);
		if(wall.existWall(E_DirLeft)) tmp.addWall(E_DirFront);
	} else if(ang == E_AngleDown){
		if(wall.existWall(E_DirFront)) tmp.addWall(E_DirBack);
		if(wall.existWall(E_DirRight)) tmp.addWall(E_DirLeft);
		if(wall.existWall(E_DirBack)) tmp.addWall(E_DirFront);
		if(wall.existWall(E_DirLeft)) tmp.addWall(E_DirRight);
	} else {
		if(wall.existWall(E_DirFront)) tmp.addWall(E_DirLeft);
		if(wall.existWall(E_DirRight)) tmp.addWall(E_DirFront);
		if(wall.existWall(E_DirBack)) tmp.addWall(E_DirRight);
		if(wall.existWall(E_DirLeft)) tmp.addWall(E_DirBack);
	}
	return tmp;
}

Walldata Map::rotateWallToRelative(Walldata wall, EMouseAngle ang){
	Walldata tmp;
	char indata[4];
	indata[0] = wall.existWall(E_DirFront);
	indata[1] = wall.existWall(E_DirRight);
	indata[2] = wall.existWall(E_DirBack);
	indata[3] = wall.existWall(E_DirLeft);
	if(indata[(ang)%4]) tmp.addWall(E_DirFront);
	if(indata[(ang+1)%4]) tmp.addWall(E_DirRight);
	if(indata[(ang+2)%4]) tmp.addWall(E_DirBack);
	if(indata[(ang+3)%4]) tmp.addWall(E_DirLeft);
	return tmp;
}

