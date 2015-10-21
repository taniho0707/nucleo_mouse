/**
 * @file Peripherals.cpp
 */

#include "Peripherals.h"


void Peripherals::setLED(char color){
	PORTB.DDR.BIT.B5 = 1;
	PORTB.DDR.BIT.B6 = 1;
	PORTB.DDR.BIT.B7 = 1;
	if(color == 'R'){
		PORTB.DR.BIT.B6 = 1;
		PORTB.DR.BIT.B7 = 0;
		PORTB.DR.BIT.B5 = 0;
	} else if(color == 'G'){
		PORTB.DR.BIT.B6 = 0;
		PORTB.DR.BIT.B7 = 1;
		PORTB.DR.BIT.B5 = 0;
	} else if(color == 'B'){
		PORTB.DR.BIT.B6 = 0;
		PORTB.DR.BIT.B7 = 0;
		PORTB.DR.BIT.B5 = 1;
	} else {
		PORTB.DR.BIT.B6 = 0;
		PORTB.DR.BIT.B7 = 0;
		PORTB.DR.BIT.B5 = 0;
	}
}

void Peripherals::addLED(char color){
	PORTB.DDR.BIT.B5 = 1;
	PORTB.DDR.BIT.B6 = 1;
	PORTB.DDR.BIT.B7 = 1;
	if(color == 'R'){
		PORTB.DR.BIT.B6 = 1;
	} else if(color == 'G'){
		PORTB.DR.BIT.B7 = 1;
	} else if(color == 'B'){
		PORTB.DR.BIT.B5 = 1;
	} else {
		PORTB.DR.BIT.B6 = 0;
		PORTB.DR.BIT.B7 = 0;
		PORTB.DR.BIT.B5 = 0;
	}
}

int Peripherals::getSwitch(){
	int i = 0;
	if(PORT9.PORT.BIT.B2 == 0) i += 1;
	if(PORT9.PORT.BIT.B1 == 0) i += 2;
	return i;
}

void Peripherals::flickLED(char color, int t, int freq){
	int count = 0;
	while((freq == 0) || (count <= freq)){
		setLED(color);
		Timer::wait(t);
		setLED(' ');
		Timer::wait(t);
		++count;
	}
}


void Peripherals::playTone(long freq, int t){
	MTU0.TGRA = static_cast<int>(781250/freq);
	MTU0.TGRB = static_cast<int>(1562500/freq)-1;
	MTU0.TCNT = 0;
	MTU.TSTRA.BIT.CST0 = 1;
	Timer::wait(t);
	MTU.TSTRA.BIT.CST0 = 0;
	return;
}

void Peripherals::playMusic(int n){
	MTU0.TCR.BIT.CCLR = 2;
	MTU0.TCR.BIT.TPSC = 3;
	MTU0.TCR.BIT.CKEG = 0;
	MTU0.TMDR1.BIT.MD = 2;
	MTU0.TIORH.BIT.IOA = 2;
	MTU0.TIORH.BIT.IOB = 1;
	MTU0.TGRA = 625;
	MTU0.TGRB = 1250-1;
	MTU0.TCNT = 0;
	MTU.TSTRA.BIT.CST0 = 0;
	switch(n){
	case 1:
		playTone(880, 100);
		playTone(1175, 150);
		break;
	case 2:
		playTone(1174, 30);
		playTone(1661, 30);
		playTone(2093, 30);
		break;
	case 3:
		playTone(440, 160);
		Timer::wait(80);
		playTone(440, 80);
		playTone(587, 80);
		Timer::wait(80);
		playTone(740, 160);
		Timer::wait(160);
		playTone(587, 160);
		playTone(659, 160);
		playTone(740, 160);
		playTone(784, 160);
		Timer::wait(80);
		playTone(740, 80);
		Timer::wait(160);
		playTone(587, 160);
		playTone(659, 80);
		break;
	case 4:
		playTone(988, 160);
		Timer::wait(80);
		playTone(988, 80);
		playTone(1109, 160);
		playTone(1175, 160);
		Timer::wait(160);
		playTone(1109, 160);
		playTone(988, 160);
		playTone(1175, 160);
		playTone(880, 160);
		Timer::wait(80);
		playTone(880, 60);
		Timer::wait(20);
		playTone(880, 160);
		playTone(988, 160);
		playTone(880, 320);
		break;
	case 5:
		playTone(1047, 240);
		playTone(880, 120);
		Timer::wait(240);
		playTone(1047, 120);
		playTone(880, 240);
		Timer::wait(120);
		playTone(1175, 240);
		playTone(1109, 120);
		playTone(1047, 120);
		Timer::wait(240);
		playTone(1760, 120);
		Timer::wait(240);
		playTone(1397, 120);
		Timer::wait(240);
		playTone(1047, 120);
		break;
	case 6:
		playTone(932, 240);
		playTone(698, 120);
		Timer::wait(240);
		playTone(1175, 120);
		playTone(932, 120);
		Timer::wait(240);
		playTone(659, 120);
		playTone(698, 120);
		playTone(784, 120);
		playTone(698, 120);
		Timer::wait(120);
		playTone(698, 110);
		Timer::wait(10);
		playTone(698, 120);
		playTone(880, 120);
		playTone(784, 120);
		playTone(698, 120);
		break;
	case 7:
		playTone(698, 130);
		playTone(784, 130);
		playTone(880, 130);
		playTone(784, 130);
		playTone(880, 130);
		playTone(988, 130);
		playTone(1047, 260);
		playTone(784, 130);
		playTone(659, 260);
		Timer::wait(130);
		playTone(698, 130);
		playTone(784, 130);
		playTone(880, 130);
		playTone(784, 130);
		playTone(880, 130);
		playTone(988, 130);
		playTone(1047, 260);
		Timer::wait(130);
		playTone(784, 260);
		Timer::wait(130);
		playTone(698, 130);
		playTone(784, 130);
		playTone(880, 130);
		playTone(784, 130);
		playTone(880, 130);
		playTone(988, 130);
		playTone(1047, 260);
		playTone(784, 130);
		playTone(660, 260);
		playTone(1568, 130);
		playTone(1397, 260);
		playTone(1319, 130);
		playTone(1175, 260);
		playTone(1319, 130);
		playTone(1047, 260);
		Timer::wait(130);
		playTone(2093, 390);
		break;
	case 8:
		playTone(698, 130);
		playTone(784, 130);
		playTone(880, 130);
		playTone(784, 130);
		playTone(880, 130);
		playTone(988, 130);
		playTone(1047, 260);
		playTone(784, 130);
		playTone(660, 260);
		playTone(1568, 130);
		playTone(1397, 260);
		playTone(1319, 130);
		playTone(1175, 260);
		playTone(1319, 130);
		playTone(1047, 260);
		Timer::wait(130);
		playTone(2093, 390);
		break;
	case 9:
		playTone(1397, 80);
		playTone(1568, 80);
		playTone(1760, 80);
		playTone(1175, 80);
		playTone(1319, 80);
		playTone(1397, 80);
		playTone(1047, 400);
		break;
	case 10:
		playTone(1975, 50);
		Timer::wait(30);
		playTone(1975, 50);
		Timer::wait(30);
		playTone(1975, 50);
		Timer::wait(100);
		playTone(1975, 50);
		Timer::wait(30);
		playTone(1975, 50);
		Timer::wait(30);
		playTone(1975, 50);
		Timer::wait(100);
		playTone(1975, 50);
		Timer::wait(30);
		playTone(1975, 50);
		Timer::wait(30);
		playTone(1975, 50);
		break;
	case 11:
		playTone(784, 100);
		playTone(1047, 100);
		playTone(1319, 100);
		playTone(1568, 100);
		Timer::wait(180);
		playTone(2093, 100);
		Timer::wait(100);
		playTone(1568, 100);
		Timer::wait(100);
		break;
	case 12:
		playTone(2093, 140);
		Timer::wait(140);
		playTone(1976, 140);
		playTone(1865, 140);
		Timer::wait(140);
		playTone(1760, 140);
		playTone(1661, 140);
		Timer::wait(140);
		playTone(1245, 280);
		Timer::wait(140);
		playTone(1661, 140);
		playTone(1568, 140);
		Timer::wait(700);
		playTone(3136, 140);
		Timer::wait(140);
		break;
	case 13:
		playTone(1568, 320);
		playTone(1397, 160);
		playTone(1245, 160);
		playTone(1175, 160);
		playTone(932, 160);
		playTone(784, 300);
		Timer::wait(20);
		playTone(1047, 160);
		playTone(1175, 160);
		playTone(1245, 160);
		playTone(1397, 160);
		playTone(1175, 320);
		Timer::wait(320);
		break;
	default:
		playTone(1568, 50);
	}
}


void Peripherals::playMorseLong(){
	playTone(880, 240);
	Timer::wait(80);
}

void Peripherals::playMorseShort(){
	playTone(880, 80);
	Timer::wait(80);
}

void Peripherals::playMorse(char i){
	if(i <= '9' && i >= '0'){
		if(i <= '5'){
			for(int j=0; j<(i-'0'); ++j) playMorseShort();
			for(int j=0; j<(5-(i-'0')); ++j) playMorseLong();
		} else {
			for(int j=0; j<((i-'0')-5); ++j) playMorseLong();
			for(int j=0; j<(10-(i-'0')); ++j) playMorseShort();
		}
	}
	Timer::wait(240);
}

void Peripherals::playMorses(char *data, int count){
	for(int i=0; i<count; ++i){
		playMorse(*data);
		++data;
	}
}


void Peripherals::put1byte(char t){
	SCI1.TDR = t;
	while(SCI1.SSR.BIT.TEND == 0){}
}

void Peripherals::putnbyte(char *t, int len){
	for(int c=0; c<len; c++){
		put1byte(t[c]);
	}
}

int Peripherals::myprintf(const char *fmt, ...){
	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;
	SCI1.SMR.BIT.CKS = 0;				//クロック選択
	SCI1.SMR.BIT.MP = 0;
	SCI1.SMR.BIT.STOP = 0;				//ストップビットは1ビット
	SCI1.SMR.BIT.PE = 0;				//パリティ無し
	SCI1.SMR.BIT.CHR = 0;				//8ビット送信
	SCI1.SMR.BIT.CM = 0;				//調歩同期
	SCI1.SCR.BIT.CKE = 0;				//SCKは入出力ポート
	SCI1.SCR.BIT.TEIE = 0;				//割り込みしない
	SCI1.SCR.BIT.TE = 0;				//送信を禁止
	SCI1.SCR.BIT.RE = 0;				//受信を禁止
	SCI1.SCR.BIT.RIE = 0;				//受信割り込みをしない
	SCI1.SCR.BIT.TIE = 0;				//送信割り込みをしない
//	SCI1.BRR = 40;						//ボーレートを38400bpsにする
	SCI1.BRR = 9;					//ボーレートを156290bpsにする
	SCI1.SEMR.BIT.ABCS = 0;
	SCI1.SCR.BIT.TE = 1;				//送信を許可
	static char buffer[100];
	int len;

	va_list ap;
	va_start(ap, fmt);

	len = vsprintf(buffer, fmt, ap);//データの長さを識別
	putnbyte(buffer, len);
	va_end(ap);
	return len;
}

