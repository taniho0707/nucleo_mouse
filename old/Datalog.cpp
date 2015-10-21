/**
 * @file Datalog.cpp
 */

#include "Datalog.h"


unsigned short *Datalog::mazePos;
uint32_t *Datalog::pos;
int Datalog::datapos = 0;
int Datalog::enabled = 0;
int Datalog::count = 0;


void Datalog::setup(){
	R_FlashDataAreaAccess(0xFFFF, 0xFFFF);
}

void Datalog::interrupt(){

}

void Datalog::cleanup(){
	R_FlashErase(BLOCK_DB0);
	R_FlashErase(BLOCK_DB1);
	R_FlashErase(BLOCK_DB2);
	R_FlashErase(BLOCK_DB3);
	R_FlashErase(BLOCK_DB4);
	R_FlashErase(BLOCK_DB5);
	R_FlashErase(BLOCK_DB6);
	R_FlashErase(BLOCK_DB7);
	R_FlashErase(BLOCK_DB8);
	R_FlashErase(BLOCK_DB9);
	R_FlashErase(BLOCK_DB10);
	R_FlashErase(BLOCK_DB11);
	R_FlashErase(BLOCK_DB12);
	R_FlashErase(BLOCK_DB13);
	R_FlashErase(BLOCK_DB14);
	R_FlashErase(BLOCK_DB15);
//	uint32_t data[10] = {0};
//	for(uint32_t i=0x00100000; i<0x00108000; i+=0x00000008)
//		R_FlashWrite(i, (uint32_t)(data), 16);
}

void Datalog::cleanupWithoutOneMaze(int m){
	Map maze;
	Datalog::loadMaze(maze, m);
	Datalog::cleanup();
	Datalog::saveMaze(maze, m);
}

int Datalog::saveData(int a, int b, int c, int d){
	if(datapos >= 800) return 1;
	uint32_t data[5] = {(uint32_t)a, (uint32_t)b, (uint32_t)c, (uint32_t)d};
	R_FlashWrite(0x00100810+(0x00000010*datapos), (uint32_t)(data), 16);
	++ datapos;
	return 0;
}

void Datalog::loadData(int n, int *data){
	pos = (uint32_t *)(0x00100810 + (0x00000010 * n));
	data[0] = (int)(/*-2147483648*/ + ((*pos)&0xFFFFFFFF));
	++ pos;
	data[1] = (int)((*pos));
	++ pos;
	data[2] = (int)((*pos)&0xFFFFFFFF);
	++ pos;
	data[3] = (int)((*pos));
}

void Datalog::clearData(){

}


void Datalog::start(int count){

}


int Datalog::loadBuildinParameter(void){

}

int Datalog::saveBuildParameter(void){

}

int Datalog::saveMaze(Map &map, int n){
	R_FlashWrite(0x00100000+(96*n), (uint32_t)(map.column), 32);
	R_FlashWrite(0x00100020+(96*n), (uint32_t)(map.row), 32);
	R_FlashWrite(0x00100040+(96*n), (uint32_t)(map.reached), 32);
	return 0;
}

int Datalog::loadMaze(Map &map, int n){
	mazePos = (unsigned short *)(0x00100000+(96*n));
	for(int i=0; i<16; ++i){
		map.column[i] = *mazePos;
		++ mazePos;
	}
	mazePos = (unsigned short *)(0x00100020+(96*n));
	for(int i=0; i<15; ++i){
		map.row[i] = *mazePos;
		++ mazePos;
	}
	mazePos = (unsigned short *)(0x00100040+(96*n));
	for(int i=0; i<16; ++i){
		map.reached[i] = *mazePos;
		++ mazePos;
	}
	return 0;
}




