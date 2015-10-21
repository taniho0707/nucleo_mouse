/**
 * @file Control.cpp
 */

#include "Control.h"


int Control::mode = 0;
int Control::submode = 0;
int Control::isFirst = 1;

int Control::x = 0;
int Control::y = 0;

Pair<char> Control::goal;


void Control::modeselect(){
	float integralR = 0, integralL = 0;
	goal.x = 7;
	goal.y = 7;
	mode = 0;
	submode = 0;
	Sensor::start();
	Timer::start();
	Encoder::start();
	if(isFirst){
		Peripherals::playMusic(1);
		isFirst = 0;
	}
	Timer::wait(1);
	if(Sensor::getBattery() < 6.8){
		Peripherals::setLED('R');
		Peripherals::playMusic(12);
		Peripherals::flickLED('R', 0, 200);
	} else if(Sensor::getBattery() < 7.2){
		Peripherals::setLED('R');
		Peripherals::playMusic(10);
		while(Peripherals::getSwitch() != 3){ }
		Peripherals::setLED(' ');
		Timer::wait(1000);
		Peripherals::playMusic(11);
	}

	while((Peripherals::getSwitch()&1) == 0){
		integralL += Encoder::getVelocity(E_MotorLeft);
		integralR += Encoder::getVelocity(E_MotorRight);
		if(Motor::absf(integralL) > 10000){
			if(integralL > 0) ++mode;
			else mode += (NUM_MODE-1);
			submode = 0;
			integralR = 0;
			integralL = 0;
			Peripherals::playMusic(999);
			if(mode % NUM_MODE == 0){
				Timer::wait(30);
				Peripherals::playMusic(999);
			}
		} else if(Motor::absf(integralR) > 10000){
			int _mode = mode % NUM_MODE;
			switch(_mode){
			case 0:
				if(integralR > 0) ++submode;
				else submode += (NUM_MODE0-1);
				integralR = 0;
				Peripherals::playMusic(999);
				if(submode % NUM_MODE0 == 0){
					Timer::wait(30);
					Peripherals::playMusic(999);
				}
				break;
			case 1:
				if(integralR > 0) ++submode;
				else submode += (NUM_MODE1-1);
				integralR = 0;
				Peripherals::playMusic(999);
				if(submode % NUM_MODE1 == 0){
					Timer::wait(30);
					Peripherals::playMusic(999);
				}
				break;
			case 2:
				if(integralR > 0) ++submode;
				else submode += (NUM_MODE2-1);
				integralR = 0;
				Peripherals::playMusic(999);
				if(submode % NUM_MODE2 == 0){
					Timer::wait(30);
					Peripherals::playMusic(999);
				}
				break;
			case 3:
				if(integralR > 0) ++submode;
				else submode += (NUM_MODE3-1);
				integralR = 0;
				Peripherals::playMusic(999);
				if(submode % NUM_MODE3 == 0){
					Timer::wait(30);
					Peripherals::playMusic(999);
				}
				break;
			case 4:
				if(integralR > 0) ++submode;
				else submode += (NUM_MODE4-1);
				integralR = 0;
				Peripherals::playMusic(999);
				if(submode % NUM_MODE4 == 0){
					Timer::wait(30);
					Peripherals::playMusic(999);
				}
				break;
			case 5:
				if(integralR > 0) ++submode;
				else submode += (NUM_MODE5-1);
				integralR = 0;
				Peripherals::playMusic(999);
				if(submode % NUM_MODE5 == 0){
					Timer::wait(30);
					Peripherals::playMusic(999);
				}
				break;
			default:
				break;
			}
		}
	}
	int left_mode_num=0, right_mode_num=0;
	integralL = 0;
	integralR = 0;
	while((Peripherals::getSwitch()&2) == 0){
		integralL += Encoder::getVelocity(E_MotorLeft);
		integralR += Encoder::getVelocity(E_MotorRight);
		if(Motor::absf(integralL) > 10000){
			if(integralL > 0) ++left_mode_num;
			else left_mode_num += 9;
			integralL = 0;
			Peripherals::playMusic(999);
			if(left_mode_num % 10 == 0){
				Timer::wait(20);
				Peripherals::playMusic(999);
			}
		}
		if(Motor::absf(integralR) > 10000){
			if(integralR > 0) ++right_mode_num;
			else submode += 9;
			integralR = 0;
			Peripherals::playMusic(999);
			if(right_mode_num % 10 == 0){
				Timer::wait(20);
				Peripherals::playMusic(999);
			}
		}
	}
	int _mode = mode % NUM_MODE;
	switch(_mode){
	case 0://Explore
		if(submode%NUM_MODE0 == 0) hidarite(-1, right_mode_num, 0);
		else if(submode%NUM_MODE0 == 1) adachi(-1, right_mode_num, 0);
		else if(submode%NUM_MODE0 == 2) adachi(-1, right_mode_num, 1);
		else if(submode%NUM_MODE0 == 3) adachi(left_mode_num, right_mode_num, 0);
		else adachi(left_mode_num, right_mode_num, 1);
		break;
	case 1://Run
		if(submode%NUM_MODE1 == 0) firstrun(0.5, 0.5, 0.5, left_mode_num, right_mode_num);
		else if(submode%NUM_MODE1 == 1) firstrun(1.0, 0.5, 0.5, left_mode_num, right_mode_num);
		else if(submode%NUM_MODE1 == 2) firstrun(2.0, 0.8, 0.8, left_mode_num, right_mode_num);
		else if(submode%NUM_MODE1 == 3) firstrun(2.5, 0.8, 0.8, left_mode_num, right_mode_num);
		else firstrun(3.0, 0.8, 1.5, left_mode_num, right_mode_num);
		break;
	case 2://Reset
		Peripherals::playTone(440, 500);
		if(submode%NUM_MODE2 == 0) Datalog::cleanup();
		else Datalog::cleanupWithoutOneMaze(left_mode_num);
		break;
	case 3://Circuit
		if(submode%NUM_MODE3 == 0) circuit(1.0, 0.5, 0.5, 0);
		else if(submode%NUM_MODE3 == 1) circuit(2.0, 0.5, 0.5, 0);
		else if(submode%NUM_MODE3 == 2) circuit(2.0, 0.8, 0.8, 0);
		else if(submode%NUM_MODE3 == 3) circuit(2.5, 0.8, 0.8, 0);
		else circuit(3.0, 0.8, 0.8, 0);
		break;
	case 4://Configure
		float tmp;
		if(left_mode_num == 0) tmp = 0.5;
		else tmp = 0.8;
		if(submode%NUM_MODE4 == 0) slalommode(90, tmp);
		else if(submode%NUM_MODE4 == 1) slalommode(900, tmp);
		else if(submode%NUM_MODE4 == 2) slalommode(45, tmp);
		else if(submode%NUM_MODE4 == 3) slalommode(135, tmp);
		else if(submode%NUM_MODE4 == 4) slalommode(-90, tmp);
		else if(submode%NUM_MODE4 == 5) slalommode(-900, tmp);
		else if(submode%NUM_MODE4 == 6) slalommode(-45, tmp);
		else if(submode%NUM_MODE4 == 7) slalommode(-135, tmp);
		else if(submode%NUM_MODE4 == 8) spinmode(0);
		else if(submode%NUM_MODE4 == 9) straightmode(0.5);
		else straightmode(1.0);
		break;
	case 5://Debug
		if(submode%NUM_MODE5 == 0) sensorcheck();
		else if(submode%NUM_MODE5 == 1) showLogdata();
		else if(submode%NUM_MODE5 == 2){
			showMazedata(0);
			showMazedata(1);
			showMazedata(2);
			showMazedata(3);
		}
		else if(submode%NUM_MODE5 == 3) showPathdata(0);
		else sensorcheck();
		break;
	default:
		break;
	}
}

void Control::test(){
	Timer::start();
	Peripherals::setLED('B');
	Encoder::start();
	Sensor::start();

	Peripherals::setLED('R');
	while(1){
		if(Sensor::existWall(E_Left)==1 && Sensor::existWall(E_LFront)==1 && Sensor::existWall(E_RFront)==1 && Sensor::existWall(E_Right)==1) break;
	}

	SpeedParameter params = {
		/*float straight */ 0.5,
		/*float oblique  */ 0.5,
		/*float turn90   */ 0.5,
		/*float turn180  */ 0.5,
		/*float turn45   */ 0.5,
		/*float turn135  */ 0.5,
		/*float oblique90*/ 0.5,
		/*float normal   */ 0.5
	};
	Walldata wall;
	Map map;
	Path p;
	Datalog::loadMaze(map, 0);
	Pair<char> currentCoordinate;
	Pair<char> targetCoordinate;
	MouseMotion motion;

	Peripherals::playMusic(9);
	Datalog::cleanup();
	Timer::wait(500);
	Sensor::resetGyro();

	Peripherals::addLED('B');

	Peripherals::flickLED('R', 100, 5);

	Motor::start();
	Motor::resetIntegral();
	Motor::setLinBothendsVelocity(0, 0.5, 0.08);		//これで，今(x, y) = (0, 1)にいることになる。示す座標は次通る箇所。
	Peripherals::setLED('B');
//	Motor::linTrapezoid(1.08, 0.5);

	currentCoordinate.x = 0;
	currentCoordinate.y = 1;
	Position::setPosition(currentCoordinate, E_AngleUp);

	p.data[0] = EMotion_Straight;
	p.division[0] = 1;
	p.data[1] = EMotion_Right135i;
	p.division[1] = 1;
	p.data[2] = EMotion_ObliqueLeft90;
	p.division[2] = 1;
	p.data[3] = EMotion_Right45o;
	p.division[3] = 1;
	p.data[2] = EMotion_Right135i;
	p.division[2] = 1;
	p.data[3] = EMotion_Oblique;
	p.division[3] = 1;
	p.data[4] = EMotion_Right45o;
	p.division[4] = 1;
	p.data[4] = EMotion_null;

	for(int i=0; i<15; ++i){
		motion = p.getNext();
		motion.speed = 0.5;
		Mouse::move(motion, params);
		if(motion.currentMotion == EMotion_null) break;
	}

	Motor::stop();
	Encoder::stop();

	Peripherals::playMusic(8);

	Timer::wait(2000);
	while(!Peripherals::getSwitch());
	showLogdata();
	while(1);

	Timer::stop();
}

void Control::sensorcheck(){
	Peripherals::setLED('B');
	Timer::wait(10);
	Peripherals::setLED(' ');
	Timer::wait(1000);
	Peripherals::setLED('B');
	Peripherals::myprintf("%f\n\r", Sensor::getBattery());
	while(1){
		Peripherals::myprintf("FL: %d, L: %d, R: %d, FR: %d, GRad: %f\n\r", Sensor::getValue(E_LFront), Sensor::getValue(E_Left), Sensor::getValue(E_Right), Sensor::getValue(E_RFront), Sensor::getGyroRad());
		Peripherals::myprintf("%d, %d, %d ", Sensor::existWall(E_Left), Sensor::existWall(E_LFront), Sensor::existWall(E_Right));
//		datalog.setData4(Sensor::getValue(E_LFront), Sensor::getValue(E_Left), Sensor::getValue(E_Right), Sensor::getValue(E_RFront));
		Timer::wait(100);
		if(Peripherals::getSwitch() == 2){
			while(1){
				Peripherals::myprintf("Gyro: %f, ZeroGyro: %f, IntegralGyro: %f\n", Sensor::getGyroRad(), Sensor::getGyroZero(), Sensor::getGyroIntegral());
				Timer::wait(100);
			}
			Timer::wait(100);
		} else if(Peripherals::getSwitch() == 1){
			Timer::wait(1000);
			Peripherals::playMorse('0'+(Sensor::getValue(E_LFront)/1000%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_LFront)/100%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_LFront)/10%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_LFront)%10));
			Timer::wait(1000);
			Peripherals::playMorse('0'+(Sensor::getValue(E_Left)/1000%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_Left)/100%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_Left)/10%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_Left)%10));
			Timer::wait(1000);
			Peripherals::playMorse('0'+(Sensor::getValue(E_Right)/1000%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_Right)/100%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_Right)/10%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_Right)%10));
			Timer::wait(1000);
			Peripherals::playMorse('0'+(Sensor::getValue(E_RFront)/1000%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_RFront)/100%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_RFront)/10%10));
			Peripherals::playMorse('0'+(Sensor::getValue(E_RFront)%10));
			Timer::wait(1000);
		}
	}
}

void Control::showLogdata(){
	int data[4];
	Peripherals::flickLED('R', 80, 3);
	for(int i=0; i<800; ++i){
		Datalog::loadData(i, data);
		Peripherals::myprintf("%d,%d,%d,%d,%d\n", i, data[0], data[1], data[2], data[3]);
		Timer::wait(1);
	}
}

void Control::showMazedata(int n){
	Map tmpmap;
	Datalog::loadMaze(tmpmap, n);
	Peripherals::flickLED('B', 100, 3);
	Peripherals::myprintf("** No. %d Maze\n", n);
	Peripherals::myprintf("*Column\n");
	for(int i=0; i<16; ++i)
		Peripherals::myprintf("\t%d\n", tmpmap.column[15-i]);
	Peripherals::myprintf("*Row\n");
	for(int i=0; i<15; ++i)
		Peripherals::myprintf("\t%d\n", tmpmap.row[14-i]);
	Peripherals::myprintf("*Reached\n");
	for(int i=0; i<16; ++i)
		Peripherals::myprintf("\t%d\n", tmpmap.reached[15-i]);
	Peripherals::myprintf("Maze Number is %d\n", n);
	Peripherals::myprintf("+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n");
	for(int i=0; i<16; ++i){
		for(int j=0; j<16; ++j){
			if(tmpmap.isExistWall(j, 15-i, E_AngleLeft)) Peripherals::myprintf("|");
			else Peripherals::myprintf(" ");
			if(tmpmap.hasReached(j, 15-i)) Peripherals::myprintf("()");
			else Peripherals::myprintf("  ");
		}
		Peripherals::myprintf("|\n");
		for(int j=0; j<16; ++j){
			Peripherals::myprintf("+");
			if(tmpmap.isExistWall(j, 15-i, E_AngleDown)) Peripherals::myprintf("--");
			else Peripherals::myprintf("  ");
		}
		Peripherals::myprintf("+\n");
	}
	return;
}

void Control::showPathdata(int m){
	Map tmpmap;
	Datalog::loadMaze(tmpmap, m);
	Path p;
	Algorithm algo = Algorithm(goal, tmpmap);
	Peripherals::setLED('G');
	algo.getPath(p, 0.5, 1);
	Peripherals::flickLED('G', 100, 3);
	Peripherals::myprintf("** No. %d Path\n", m);
	for(int i=0; i<256; ++i){
		if(p.data[i] == EMotion_null){
			Peripherals::myprintf("EMotion_null       : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Straight){
			Peripherals::myprintf("EMotion_Straight   : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Right90){
			Peripherals::myprintf("EMotion_Right90    : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Left90){
			Peripherals::myprintf("EMotion_Left90     : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Turn180){
			Peripherals::myprintf("EMotion_Turn180    : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_BigRight90){
			Peripherals::myprintf("EMotion_BigRight90 : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_BigLeft90){
			Peripherals::myprintf("EMotion_BigLeft90  : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Right135i){
			Peripherals::myprintf("EMotion_Right135i  : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Left135i){
			Peripherals::myprintf("EMotion_Left135i   : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_BigRight180){
			Peripherals::myprintf("EMotion_BigRight180: %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_BigLeft180){
			Peripherals::myprintf("EMotion_BigLeft180 : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Right135o){
			Peripherals::myprintf("EMotion_Right135o  : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Left135o){
			Peripherals::myprintf("EMotion_Left135o   : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Right45i){
			Peripherals::myprintf("EMotion_Right45i   : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Left45i){
			Peripherals::myprintf("EMotion_Left45i    : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Right45o){
			Peripherals::myprintf("EMotion_Right45o   : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Left45o){
			Peripherals::myprintf("EMotion_Left45o    : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_Oblique){
			Peripherals::myprintf("EMotion_Oblique    : %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_ObliqueRight90){
			Peripherals::myprintf("EMotion_ObliqueRight90: %d\n", p.division[i]);
		} else if(p.data[i] == EMotion_ObliqueLeft90){
			Peripherals::myprintf("EMotion_ObliqueLeft90 : %d\n", p.division[i]);
		} else {
			Peripherals::myprintf("EMotion_stop       : %d\n", p.division[i]);
		}
	}
}

void Control::slalommode(int angle, float v){
	if(v == 0.5){
		SpeedParameter params = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
		if(angle == 900) {
			Peripherals::playMusic(4);
			Timer::wait(500);
			Sensor::resetGyro();
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.5, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_BigRight90, params, 1);
			Mouse::moveOnce(EMotion_Straight, params, 2);
			Mouse::moveOnce(EMotion_stop, params, 1);
			Motor::resetIntegral();
			Peripherals::setLED('R');
			while(1){

			}
		} else if(angle == -900) {
			Peripherals::playMusic(4);
			Timer::wait(500);
			Sensor::resetGyro();
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.5, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_BigLeft90, params, 1);
			Mouse::moveOnce(EMotion_Straight, params, 2);
			Mouse::moveOnce(EMotion_stop, params, 1);
			Motor::resetIntegral();
			Peripherals::setLED('R');
			while(1){

			}
		} else if(angle == 90){
			Peripherals::playMusic(5);
			Timer::wait(500);
			Sensor::resetGyro();
	//		Datalog::start(10);
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.5, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Right90, params, 1);
			Mouse::moveOnce(EMotion_Straight, params, 1);
			Mouse::moveOnce(EMotion_stop, params, 1);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		} else if(angle == -90){
			Peripherals::playMusic(6);
			Timer::wait(500);
			Sensor::resetGyro();
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.5, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Left90, params, 1);
			Mouse::moveOnce(EMotion_Straight, params, 1);
			Mouse::moveOnce(EMotion_stop, params, 1);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		} else if(angle == 45){
			Peripherals::playMusic(6);
			Timer::wait(500);
			Sensor::resetGyro();
	//		Datalog::start(10);
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.5, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Right45i, params, 1);
			Mouse::moveOnce(EMotion_Oblique, params, 1);
			Motor::setLinBothendsVelocity(0.5, 0.0, 0.1272);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		} else if(angle == -45){
			Peripherals::playMusic(6);
			Timer::wait(500);
			Sensor::resetGyro();
	//		Datalog::start(10);
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.5, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Left45i, params, 1);
			Mouse::moveOnce(EMotion_Oblique, params, 1);
			Motor::setLinBothendsVelocity(0.5, 0.0, 0.1272);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		} else if(angle == 135){
			Peripherals::playMusic(6);
			Timer::wait(500);
			Sensor::resetGyro();
	//		Datalog::start(10);
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.5, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Right135i, params, 1);
			Mouse::moveOnce(EMotion_Oblique, params, 1);
			Motor::setLinBothendsVelocity(0.5, 0.0, 0.1272);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		} else if(angle == -135){
			Peripherals::playMusic(6);
			Timer::wait(500);
			Sensor::resetGyro();
	//		Datalog::start(10);
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.5, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Left135i, params, 1);
			Mouse::moveOnce(EMotion_Oblique, params, 1);
			Motor::setLinBothendsVelocity(0.5, 0.0, 0.1272);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		}
	} else if(v == 0.8){
		SpeedParameter params = {0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8};
		if(angle == 900) {
			Peripherals::playMusic(4);
			Timer::wait(500);
			Sensor::resetGyro();
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.8, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_BigRight90, params, 1);
			Mouse::moveOnce(EMotion_Straight, params, 2);
			Mouse::moveOnce(EMotion_stop, params, 1);
			Motor::resetIntegral();
			Peripherals::setLED('R');
			while(1){

			}
		} else if(angle == -900) {
			Peripherals::playMusic(4);
			Timer::wait(500);
			Sensor::resetGyro();
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.8, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_BigLeft90, params, 1);
			Mouse::moveOnce(EMotion_Straight, params, 2);
			Mouse::moveOnce(EMotion_stop, params, 1);
			Motor::resetIntegral();
			Peripherals::setLED('R');
			while(1){

			}
		} else if(angle == 90){
			Peripherals::playMusic(5);
			Timer::wait(500);
			Sensor::resetGyro();
	//		Datalog::start(10);
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.8, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Right90, params, 1);
			Mouse::moveOnce(EMotion_Straight, params, 1);
			Mouse::moveOnce(EMotion_stop, params, 1);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		} else if(angle == -90){
			Peripherals::playMusic(6);
			Timer::wait(500);
			Sensor::resetGyro();
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.8, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Left90, params, 1);
			Mouse::moveOnce(EMotion_Straight, params, 1);
			Mouse::moveOnce(EMotion_stop, params, 1);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		} else if(angle == 45){
			Peripherals::playMusic(6);
			Timer::wait(500);
			Sensor::resetGyro();
	//		Datalog::start(10);
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.8, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Right45i, params, 1);
			Mouse::moveOnce(EMotion_Oblique, params, 1);
			Motor::setLinBothendsVelocity(0.8, 0.0, 0.1272);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		} else if(angle == -45){
			Peripherals::playMusic(6);
			Timer::wait(500);
			Sensor::resetGyro();
	//		Datalog::start(10);
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.8, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Left45i, params, 1);
			Mouse::moveOnce(EMotion_Oblique, params, 1);
			Motor::setLinBothendsVelocity(0.8, 0.0, 0.1272);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		} else if(angle == 135){
			Peripherals::playMusic(6);
			Timer::wait(500);
			Sensor::resetGyro();
	//		Datalog::start(10);
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.8, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Right135i, params, 1);
			Mouse::moveOnce(EMotion_Oblique, params, 1);
			Motor::setLinBothendsVelocity(0.8, 0.0, 0.1272);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		} else if(angle == -135){
			Peripherals::playMusic(6);
			Timer::wait(500);
			Sensor::resetGyro();
	//		Datalog::start(10);
			Motor::start();
			Motor::resetIntegral();
			Motor::setLinBothendsVelocity(0, 0.8, 0.04);
			Mouse::moveOnce(EMotion_Straight, params, 3);
			Mouse::moveOnce(EMotion_Left135i, params, 1);
			Mouse::moveOnce(EMotion_Oblique, params, 1);
			Motor::setLinBothendsVelocity(0.8, 0.0, 0.1272);
			while(Peripherals::getSwitch() == 0){
				Peripherals::setLED(' ');
				Timer::wait(1);
			}
			Motor::stop();
		}
	}
}

void Control::straightmode(float v){
	SpeedParameter params = {v, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
	Peripherals::playMusic(13);
	Timer::wait(500);
	Sensor::resetGyro();
	Motor::start();
	Motor::resetIntegral();
	Mouse::moveOnce(EMotion_Start, params, 1);
	Mouse::moveOnce(EMotion_Straight, params, 6);
	Mouse::moveOnce(EMotion_stop, params, 1);
	Motor::setLinVelocity(0);
	Motor::setRadVelocity(0);
//	Motor::stop();
	while(1){
		if(Peripherals::getSwitch()){
			Timer::wait(1000);
		}
	}
}

void Control::spinmode(int m){
	SpeedParameter params = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
	Peripherals::playMusic(13);
	Timer::wait(500);
	Sensor::resetGyro();
	Motor::start();
	Motor::resetIntegral();
	Mouse::moveOnce(EMotion_OnlyTurn180, params, 1);
	Timer::wait(500);
	Mouse::moveOnce(EMotion_OnlyTurn180, params, 1);
	Timer::wait(500);
	Mouse::moveOnce(EMotion_OnlyTurn180, params, 1);
	Timer::wait(500);
	Mouse::moveOnce(EMotion_OnlyTurn180, params, 1);
	Timer::wait(500);
	Mouse::moveOnce(EMotion_OnlyTurn180, params, 1);
	Timer::wait(500);
	Mouse::moveOnce(EMotion_OnlyTurn180, params, 1);
	Timer::wait(500);
	Mouse::moveOnce(EMotion_OnlyTurn180, params, 1);
	Timer::wait(500);
	Mouse::moveOnce(EMotion_OnlyTurn180, params, 1);
	Timer::wait(5000);
	while(1){
		if(Peripherals::getSwitch()){
			Timer::wait(1000);
		}
	}
}

void Control::hidarite(int loadmaze, int savemaze, int m){
	Peripherals::setLED('G');
	Algorithm algo;
	MouseMotion motion;
	SpeedParameter params = {
		/*float straight */ 0.5,
		/*float oblique  */ 0.5,
		/*float turn90   */ 0.5,
		/*float turn180  */ 0.5,
		/*float turn45   */ 0.5,
		/*float turn135  */ 0.5,
		/*float oblique90*/ 0.5,
		/*float normal   */ 0.5
	};

	Peripherals::setLED('G');
	while(1){
		if(Sensor::existWall(E_LFront)==1 && Sensor::existWall(E_Left)==1 && Sensor::existWall(E_Right)==1 && Sensor::existWall(E_RFront)==1) break;
	}

	Walldata wall;
	Map map;
	if(loadmaze >= 0 && loadmaze < 4){
		Datalog::loadMaze(map, loadmaze);
	}

	Peripherals::playTone(1047, 100);
	Peripherals::playTone(1319, 100);
	Peripherals::playTone(1397, 100);
	Peripherals::playTone(1567, 50);
	Timer::wait(100);
	Peripherals::playTone(2093, 100);
	Timer::wait(100);
	Peripherals::playTone(1568, 100);
	Timer::wait(500);
	Peripherals::setLED(' ');
	Sensor::resetGyro();
	Peripherals::flickLED('G', 100, 5);
	Motor::start();
	Motor::resetIntegral();

	Motor::setLinBothendsVelocity(0, 0.5, 0.08);
	Peripherals::setLED('B');

	while(1){
		wall = Sensor::getWall();
		motion = algo.hidarite(wall, 0);
		Peripherals::setLED(' ');
		motion.division = 2;
		Mouse::move(motion, params);
		algo.mapdata.setReached(Position::x, Position::y);
		if(motion.currentMotion == EMotion_null) break;
	}
	Datalog::saveMaze(algo.mapdata, savemaze);

	Motor::stop();
	Encoder::stop();
	Timer::stop();

}

void Control::adachi(int loadmaze, int savemaze, int mode){
	Peripherals::setLED('G');
	while(1){
		if(Sensor::existWall(E_Left)==1 && Sensor::existWall(E_LFront)==1 && Sensor::existWall(E_RFront)==1 && Sensor::existWall(E_Right)==1) break;
	}

	SpeedParameter params = {
		/*float straight */ 0.5,
		/*float oblique  */ 0.5,
		/*float turn90   */ 0.5,
		/*float turn180  */ 0.5,
		/*float turn45   */ 0.5,
		/*float turn135  */ 0.5,
		/*float oblique90*/ 0.5,
		/*float normal   */ 0.5
	};
	Walldata wall;
	Map map;
	if(loadmaze >= 0 && loadmaze < 4){
		Datalog::loadMaze(map, loadmaze);
	}
	Algorithm algo(goal, map);
	Pair<char> currentCoordinate;
	Pair<char> targetCoordinate;
	MouseMotion motion;

	Peripherals::playMusic(9);
	Timer::wait(500);
	Sensor::resetGyro();
	Peripherals::flickLED('G', 100, 5);

	Motor::start();
	Motor::resetIntegral();
	Motor::setLinBothendsVelocity(0, 0.5, 0.13);		//これで，今(x, y) = (0, 1)にいることになる。示す座標は次通る箇所。
	Peripherals::setLED('G');

	currentCoordinate.x = 0;
	currentCoordinate.y = 1;
	Position::setPosition(currentCoordinate, E_AngleUp);
	algo.mapdata.setReached(Position::x, Position::y);

	while(1){
		wall = Sensor::getWall();
		motion = algo.adachi(wall, 0, 0);
		Peripherals::setLED(' ');
		motion.division = 2;
		Mouse::move(motion, params);
		algo.mapdata.setReached(Position::x, Position::y);
		if(motion.currentMotion == EMotion_null) break;
	}

	Datalog::saveMaze(algo.mapdata, savemaze);
	Peripherals::playMusic(6);
	Timer::wait(500);

	if(mode != 0){
		Peripherals::playMusic(13);
		goal.x = 0;
		goal.y = 0;
		algo.goalCoordinate.x = 0;
		algo.goalCoordinate.y = 0;
		motion.currentMotion = EMotion_OnlyTurn180;
		Mouse::move(motion, params);
		Position::setNextAngle(EMotion_OnlyTurn180);
		goal.x = 0;
		goal.y = 0;
		algo.goalCoordinate.x = 0;
		algo.goalCoordinate.y = 0;
		algo.mapdata.setReached(goal.x, goal.y);
		Motor::resetIntegral();
		Motor::resetGyroIntegral();
		Motor::setLinBothendsVelocity(0.0, 0.5, 0.9);
		while(1){
			wall = Sensor::getWall();
			motion = algo.adachi(wall, 0, 1);
			Peripherals::setLED(' ');
			motion.division = 2;
			Mouse::move(motion, params);
			algo.mapdata.setReached(Position::x, Position::y);
			if(motion.currentMotion == EMotion_null) break;
		}
		Datalog::saveMaze(algo.mapdata, savemaze+1);
	}

	Timer::wait(1000);

	Motor::stop();
	Encoder::stop();
	Timer::stop();
}

void Control::firstrun(float lin_v, float turn_v, float obl_v, int loadmaze, int mode){
	Peripherals::setLED('R');
	while(1){
		if(Sensor::existWall(E_Left)==1 && Sensor::existWall(E_LFront)==1 && Sensor::existWall(E_RFront)==1 && Sensor::existWall(E_Right)==1) break;
	}

	SpeedParameter params = {
		/*float straight */ lin_v,
		/*float oblique  */ obl_v,
		/*float turn90   */ turn_v,
		/*float turn180  */ turn_v,
		/*float turn45   */ turn_v,
		/*float turn135  */ turn_v,
		/*float oblique90*/ turn_v,
		/*float normal   */ turn_v
	};
	Walldata wall;
	Map map;
	Path p;
	Datalog::loadMaze(map, loadmaze);
	Algorithm algo(goal, map);
	Pair<char> currentCoordinate;
	Pair<char> targetCoordinate;
	MouseMotion motion;

	Peripherals::playMusic(9);
	Timer::wait(500);
	Sensor::resetGyro();

	Peripherals::addLED('B');
	volatile int c = algo.getPath(p, lin_v, mode);

	Peripherals::flickLED('R', 100, 5);

	Motor::start();
	Motor::resetIntegral();
	Motor::setLinBothendsVelocity(0, params.normal, 0.04);		//これで，今(x, y) = (0, 1)にいることになる。示す座標は次通る箇所。
	Peripherals::setLED('B');

	currentCoordinate.x = 0;
	currentCoordinate.y = 1;
	Position::setPosition(currentCoordinate, E_AngleUp);

	for(int i=0; i<c; ++i){
		motion = p.getNext();
		motion.speed = 0.0;
		if(motion.currentMotion == EMotion_null) break;
		Mouse::move(motion, params);
	}
	Motor::setLinBothendsVelocity(params.straight, 0, 0.18);

	Motor::stop();
	Encoder::stop();

	Peripherals::playMusic(8);

	Timer::stop();

}


void Control::circuit(float straight_v, float turn_v, float obl_v, int size){
	Peripherals::setLED('R');
	while(1){
		if(Sensor::existWall(E_Left)==1 && Sensor::existWall(E_LFront)==1 && Sensor::existWall(E_RFront)==1 && Sensor::existWall(E_Right)==1) break;
	}
	MouseMotion motion;

	SpeedParameter params = {
		/*float straight */ straight_v,
		/*float oblique  */ obl_v,
		/*float turn90   */ turn_v,
		/*float turn180  */ turn_v,
		/*float turn45   */ turn_v,
		/*float turn135  */ turn_v,
		/*float oblique90*/ turn_v,
		/*float normal   */ turn_v
	};
	Peripherals::playMusic(13);
	Timer::wait(500);
	Sensor::resetGyro();

	Peripherals::flickLED('R', 50, 10);

	Motor::start();
	Motor::resetIntegral();
	Peripherals::setLED('R');
	Pair<char> wh;

	if(size == 0){
		wh.x = 26;
		wh.y = 26;
	} else if(size == 1){
		wh.x = 10;
		wh.y = 26;
	} else return;

	Motor::setLinBothendsVelocity(0, params.normal, 0.04);
	Mouse::moveOnce(EMotion_Straight, params, wh.y+2);
	Mouse::moveOnce(EMotion_BigRight90, params, 1);
	Mouse::moveOnce(EMotion_Straight, params, wh.x);
	Mouse::moveOnce(EMotion_BigRight90, params, 1);
	Mouse::moveOnce(EMotion_Straight, params, wh.y);
	Mouse::moveOnce(EMotion_BigRight90, params, 1);
	Mouse::moveOnce(EMotion_Straight, params, wh.x);
	Mouse::moveOnce(EMotion_BigRight90, params, 1);
	Mouse::moveOnce(EMotion_Straight, params, wh.y);
	Mouse::moveOnce(EMotion_BigRight90, params, 1);
	Mouse::moveOnce(EMotion_Straight, params, wh.x);
	Mouse::moveOnce(EMotion_BigRight90, params, 1);
	Mouse::moveOnce(EMotion_Straight, params, wh.y);
	Mouse::moveOnce(EMotion_BigRight90, params, 1);
	Mouse::moveOnce(EMotion_Straight, params, wh.x);
	Mouse::moveOnce(EMotion_BigRight90, params, 1);
	Mouse::moveOnce(EMotion_stop, params, 1);

	/* @todo サーキットモードをパスで走る
	motion.currentMotion = EMotion_Straight;
	motion.division = wh.y - 5;
	Mouse::move(motion);
	motion.currentMotion = EMotion_BigRight90;
	Mouse::move(motion);
	motion.currentMotion = EMotion_Straight;
	motion.division = wh.x - 4;
	Mouse::move(motion);
	motion.currentMotion = EMotion_BigRight90;
	Mouse::move(motion);

	for(int i=0; i<5; ++i){
		motion.currentMotion = EMotion_Straight;
		motion.division = wh.y - 4;
		Mouse::move(motion);
		motion.currentMotion = EMotion_BigRight90;
		Mouse::move(motion);
		motion.currentMotion = EMotion_Straight;
		motion.division = wh.x - 4;
		Mouse::move(motion);
		motion.currentMotion = EMotion_BigRight90;
		Mouse::move(motion);
	}
	Motor::setLinBothendsVelocity(subv, 0, 0.26);
	*/

	Timer::wait(500);

	Motor::stop();
	Encoder::stop();
	Timer::stop();
}


