/**
 * @file Sensor.cpp
 */

#include "Sensor.h"


float Sensor::battery = 0;
int Sensor::fl[5] = {0};
int Sensor::l[5] = {0};
int Sensor::r[5] = {0};
int Sensor::fr[5] = {0};
int Sensor::currentSensorPos = 0;

long Sensor::gyro = 0;
float Sensor::zeroGyro = ZERO_GYRO;
float Sensor::integralGyro = 0;
float Sensor::sensitivGyro_r = SENSITIV_GYRO_R;
float Sensor::sensitivGyro_l = SENSITIV_GYRO_L;

int Sensor::ref_fl = SENSOR_REF_FL;
int Sensor::ref_l = SENSOR_REF_L;
int Sensor::ref_r = SENSOR_REF_R;
int Sensor::ref_fr = SENSOR_REF_FR;
int Sensor::lim[4] = {SENSOR_LIM_FL, SENSOR_LIM_L, SENSOR_LIM_R, SENSOR_LIM_FR};
int Sensor::ref_obl_r = SENSOR_OBL_REF_R;
int Sensor::ref_obl_l = SENSOR_OBL_REF_L;

int Sensor::working = 0;


int Sensor::getAvgArray(int *array, int max){
	int tmp = 0;
	for(int i=0; i<max; ++i){
		tmp += *array;
		array ++;
	}
	return (int)(tmp/5);
}

int Sensor::removeNegative(int i){
	if(i >= 0) return i;
	else return 0;
}


float Sensor::getBattery(){
	return battery;
}

void Sensor::resetGyro(){
	long tmp = 0;
	long tmpr = 0, tmpl = 0;
	S12AD1.ADANS.BIT.CH = 0;
	for(int i=0; i<200; i++){
		Timer::wait(1);
	}
	for(int i=0; i<1000; i++){
		tmp += gyro;
		tmpr += getValue(E_Right);
		tmpl += getValue(E_Left);
		Timer::wait(1);
	}
	zeroGyro = ((tmp>0) ? sensitivGyro_r : sensitivGyro_l) * tmp / 1000;
	/// @TODO ジャイロ補正をいいかんじに
//	zeroGyro += 0.075;
//	ref_l = tmpl/1000 - 20;
//	ref_r = tmpr/1000 - 20;
}

float Sensor::getGyroRad(){
	return ((gyro>0) ? sensitivGyro_r : sensitivGyro_l) * gyro - zeroGyro;
}

long Sensor::get3V3(){
	S12AD1.ADANS.BIT.CH = 3;
	S12AD1.ADCSR.BIT.ADST = 1;
	while(S12AD1.ADCSR.BIT.ADST){}
	return S12AD1.ADDR3;
}

void Sensor::start(){
	PORTA.DDR.BIT.B3 = 1;
	PORTA.DDR.BIT.B4 = 1;
	PORTA.DDR.BIT.B2 = 1;
	PORTA.DDR.BIT.B5 = 1;

	IEN(S12AD0, S12ADI0) = 0;
	/// @todo センサ値の取得中に他の作業ができるようにしたい
	IPR(S12AD0, S12ADI0) = 0;//12;
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;
	S12AD0.ADCSR.BIT.ADST = 0;
	S12AD0.ADCSR.BIT.EXTRG = 0;
	S12AD0.ADCSR.BIT.CKS = 3;
	S12AD0.ADCSR.BIT.ADIE = 1;
	S12AD0.ADCSR.BIT.ADCS = 1;
	S12AD0.ADANS.BIT.CH = 3;	//1サイクルスキャンモード
	IR(S12AD0, S12ADI0) = 0;
	IEN(S12AD0, S12ADI0) = 1;

	//ジャイロを動作させる
	PORT2.DDR.BIT.B3 = 1;
	PORT2.DDR.BIT.B4 = 1;
	PORT2.DR.BIT.B3 = 0;
	PORT2.DR.BIT.B4 = 0;

	SYSTEM.MSTPCRA.BIT.MSTPA16 = 0;
	S12AD1.ADCSR.BIT.EXTRG = 0;
	S12AD1.ADCSR.BIT.CKS = 3;
	S12AD1.ADANS.BIT.CH = 0;
	S12AD1.ADCSR.BIT.ADST = 1;

	working = 1;
}

void Sensor::stop(){
	working = 0;
}

void Sensor::interrupt(){
	if(working){
		S12AD1.ADANS.BIT.CH = 1;
		S12AD1.ADCSR.BIT.ADST = 1;
		while(S12AD1.ADCSR.BIT.ADST){}
		gyro =  S12AD1.ADDR1;
		integralGyro += getGyroRad();

		S12AD1.ADANS.BIT.CH = 0;
		S12AD1.ADCSR.BIT.ADST = 1;
		while(S12AD1.ADCSR.BIT.ADST){}
		battery = 15.0 * S12AD1.ADDR0A / 4095;

		S12AD0.ADCSR.BIT.ADST = 1;
		while(S12AD0.ADCSR.BIT.ADST){}
		PORTA.DR.BIT.B3 = 1;
		PORTA.DR.BIT.B4 = 1;
		PORTA.DR.BIT.B2 = 1;
		PORTA.DR.BIT.B5 = 1;
		fl[currentSensorPos] = S12AD0.ADDR0A;
		l[currentSensorPos] = S12AD0.ADDR1;
		r[currentSensorPos] = S12AD0.ADDR2;
		fr[currentSensorPos] = S12AD0.ADDR3;
		for(int i=0; i<300; i++){}
		S12AD0.ADCSR.BIT.ADST = 1;
		while(S12AD0.ADCSR.BIT.ADST){}
		fl[currentSensorPos] = S12AD0.ADDR0A - fl[currentSensorPos];
		l[currentSensorPos] = S12AD0.ADDR1 - l[currentSensorPos];
		r[currentSensorPos] = S12AD0.ADDR2 - r[currentSensorPos];
		fr[currentSensorPos] = S12AD0.ADDR3 - fr[currentSensorPos];
		PORTA.DR.BIT.B3 = 0;
		PORTA.DR.BIT.B4 = 0;
		PORTA.DR.BIT.B2 = 0;
		PORTA.DR.BIT.B5 = 0;
		(currentSensorPos > 3) ? (currentSensorPos = 0) : (currentSensorPos ++);
	}
}

int Sensor::getValue(ESensorPosition t){
	/// @todo 平均値を取る？
	if(t == E_LFront){
//		return getAvgArray(fl, 5);
		return fl[currentSensorPos];
	} else if(t == E_Left){
//		return getAvgArray(l, 5);
		return l[currentSensorPos];
	} else if(t == E_Right){
//		return getAvgArray(r, 5);
		return r[currentSensorPos];
	} else if(t == E_RFront){
//		return getAvgArray(fr, 5);
		return fr[currentSensorPos];
	} else {
		return 0;
	}
}

int Sensor::existWall(ESensorPosition t){
	if(getValue(t) > lim[t]) return 1;
	else return 0;
}

Walldata Sensor::getWall(){
	Walldata output;
	if(existWall(E_LFront) && existWall(E_RFront)) output += 1;
	if(existWall(E_Left)) output += 2;
	if(existWall(E_Right)) output += 4;
	return output;
}

int Sensor::getCorrection(int max){
	int tmpR;
	int tmpL;

	if(Motor::isCurrentOblique()){
		tmpR = getValue(E_RFront) - ref_obl_r;
		tmpL = ref_obl_l - getValue(E_LFront);
		if(tmpR < 0) tmpR = 0;
		if(tmpL < 0) tmpL = 0;
		tmpR *= 2;
		tmpL *= 2;
		return tmpR + tmpL;
	} else {
		tmpR = getValue(E_Right) - ref_r;
		tmpL = ref_l - getValue(E_Left);
	}

	if(existWall(E_LFront) != 0 && existWall(E_RFront) != 0) return 0;

	if(r[currentSensorPos]+10 < r[(currentSensorPos==0 ? 4 : currentSensorPos-1)]){ tmpR = 0;/* tmpL *= 2; */}
	if(l[currentSensorPos]+10 < l[(currentSensorPos==0 ? 4 : currentSensorPos-1)]){ tmpL = 0;/* tmpR *= 2; */}
//	if(r[currentSensorPos]+10 < r[(currentSensorPos==4 ? 0 : currentSensorPos+1)]) tmpR = 0;
//	if(l[currentSensorPos]+10 < l[(currentSensorPos==4 ? 0 : currentSensorPos+1)]) tmpL = 0;

	if(existWall(E_Right) == 0){
//	if(tmpR < -50){
		tmpR = 0;
		tmpL *= 2;
	}
	if(existWall(E_Left) == 0){
//	if(tmpL < -50){
		tmpL = 0;
		tmpR *= 2;
	}

//	int retval = 0;
//	if(existWall(E_Left)){
//		retval = tmpL;
//	} else if(existWall(E_Right)){
//		retval = tmpR;
//	} else {
//		retval = 0;
//	}
//
	int retval = tmpR + tmpL;

	if(retval > max) return max;
	if(-1*retval < -1*max) return -max;
	return retval;
}

int Sensor::getFrontCorrection(){
	return (int)(((ref_fl-getValue(E_LFront))+(ref_fr-getValue(E_RFront))) / 2);
}

float Sensor::getGyroZero(){
	return zeroGyro;
}

float Sensor::getGyroIntegral(){
	return integralGyro;
}

