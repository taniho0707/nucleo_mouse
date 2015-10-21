/**
 * @file Encoder.cpp
 */

#include "Encoder.h"


Encoder::stateEncoder Encoder::enc_r;
Encoder::stateEncoder Encoder::enc_l;

void Encoder::start(){
	enc_r.count = 0;
	enc_r.velocity = 0;
	enc_l.count = 0;
	enc_l.velocity = 0;

	PORT3.DDR.BIT.B0 = 0;
	PORT3.DDR.BIT.B1 = 0;
	PORT3.DDR.BIT.B2 = 0;
	PORT3.DDR.BIT.B3 = 0;

	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0;
	IOPORT.PFCMTU.BIT.TCLKS = 0;
	MTU1.TCR.BIT.CCLR = 0;
	MTU2.TCR.BIT.CCLR = 0;
	MTU1.TCR.BIT.CKEG = 2;
	MTU2.TCR.BIT.CKEG = 2;
	MTU1.TMDR1.BIT.MD = 4;
	MTU2.TMDR1.BIT.MD = 4;
	PORT3.ICR.BIT.B0 = 1;
	PORT3.ICR.BIT.B1 = 1;
	PORT3.ICR.BIT.B2 = 1;
	PORT3.ICR.BIT.B3 = 1;

	MTU1.TCNT = 30000;
	MTU2.TCNT = 30000;
	MTU.TSTRA.BIT.CST1 = 1;
	MTU.TSTRA.BIT.CST2 = 1;
}

void Encoder::stop(){
	MTU.TSTRA.BIT.CST1 = 0;
	MTU.TSTRA.BIT.CST2 = 0;
}

void Encoder::interrupt(){
	enc_l.count = 30000 - MTU1.TCNT;
	MTU1.TCNT = 30000;
	enc_l.velocity = DISTANCE_PULSE * enc_l.count;
	enc_r.count = MTU2.TCNT - 30000;
	MTU2.TCNT = 30000;
	enc_r.velocity = DISTANCE_PULSE * enc_r.count;
}

long Encoder::getCount(EMotorPosition side){
	if(side == E_MotorLeft){
		return enc_l.count;
	} else if(side == E_MotorRight){
		return enc_r.count;
	} else {
		return 0;
	}
}

float Encoder::getVelocity(EMotorPosition side){
	if(side == E_MotorLeft){
		return enc_l.velocity;
	} else if(side == E_MotorRight){
		return enc_r.velocity;
	} else {
		return 0;
	}
}

