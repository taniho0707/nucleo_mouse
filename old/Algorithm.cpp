/*
 * @file Algorithm.cpp
 */

#include "Algorithm.h"


/// @todo charで良いのか？
Queue<char> que;

//Map mapdata;
//Footmap footdata;


Algorithm::Algorithm(){
	currentAngle = E_AngleUp;
	goalCoordinate.x = 7;
	goalCoordinate.y = 7;
	currentCoordinate.x = 0;
	currentCoordinate.y = 1;
	//	que = Queue();
	//	mapdata = Map();
	//	footdata = Footmap();
}

Algorithm::Algorithm(Pair<char> goal){
	currentAngle = E_AngleUp;
	goalCoordinate.x = goal.x;
	goalCoordinate.y = goal.y;
	currentCoordinate.x = 0;
	currentCoordinate.y = 1;
	//	que = Queue();
	//	mapdata = Map();
	//	footdata = Footmap();
}

Algorithm::Algorithm(Pair<char> goal, Map &map){
	currentAngle = E_AngleUp;
	goalCoordinate.x = goal.x;
	goalCoordinate.y = goal.y;
	currentCoordinate.x = 0;
	currentCoordinate.y = 1;
	//	que = Queue();
	//	mapdata = Map();
	//	footdata = Footmap();
	for(int i=0; i<16; ++i){
		mapdata.column[i] = map.column[i];
		if(i != 15) mapdata.row[i] = map.row[i];
		mapdata.reached[i] = map.reached[i];
	}
}

Algorithm::Algorithm(Pair<char> goal, Map &map, Triple<char> current){
	currentAngle = E_AngleUp;
	goalCoordinate.x = goal.x;
	goalCoordinate.y = goal.y;
	currentCoordinate.x = current.x;
	currentCoordinate.y = current.y;
	//	que = Queue();
	//	mapdata = Map();
	//	footdata = Footmap();
	for(int i=0; i<16; ++i){
		mapdata.column[i] = map.column[i];
		if(i != 15) mapdata.row[i] = map.row[i];
		mapdata.reached[i] = map.reached[i];
	}
}


MouseMotion Algorithm::hidarite(Walldata wall, int mode){
	MouseMotion motion = {
			EMotion_null,
			1,
			0,
			EMotion_null
	};
	currentCoordinate = Position::getCoordinate();
	currentAngle = Position::getAngle();
	if((currentCoordinate.x == goalCoordinate.x && currentCoordinate.y == goalCoordinate.y) || (currentCoordinate.x == 0 && currentCoordinate.y == 0)){
		motion.currentMotion = EMotion_null;
	} else if(wall.existWall(E_DirLeft) == 0){
		motion.currentMotion = EMotion_Left90;
		Position::setNextAngle(EMotion_Left90);
	} else if(wall.existWall(E_DirFront) == 0){
		motion.currentMotion = EMotion_Straight;
		Position::setNextAngle(EMotion_Straight);
	} else if(wall.existWall(E_DirRight) == 0){
		motion.currentMotion = EMotion_Right90;
		Position::setNextAngle(EMotion_Right90);
	} else {
		motion.currentMotion = EMotion_Turn180;
		Position::setNextAngle(EMotion_Turn180);
	}
	return motion;
}


MouseMotion Algorithm::adachi(Walldata wall, int count, int mode){
	MouseMotion motion = {
			EMotion_null,
			1,
			0,
			EMotion_null
	};
	currentCoordinate = Position::getCoordinate();
	currentAngle = Position::getAngle();
	mapdata.addWall(currentCoordinate.x, currentCoordinate.y, currentAngle, wall);
	if(currentCoordinate.x == goalCoordinate.x && currentCoordinate.y == goalCoordinate.y){
		motion.currentMotion = EMotion_null;
	} else {
		Triple<char> t;
		t.x = goalCoordinate.x;
		t.y = goalCoordinate.y;
		t.t = 0;
		adachiRenewFootmap(t, footdata, 0);

		if(mapdata.isExistWall(currentCoordinate.x, currentCoordinate.y, E_AngleUp)==0 && footdata.getFootmap(currentCoordinate.x, currentCoordinate.y, 512) > footdata.getFootmap(currentCoordinate.x, currentCoordinate.y+1, 512) && (mode != 10 || mapdata.hasReached(currentCoordinate.x, currentCoordinate.y+1))){
			if(currentAngle == E_AngleUp){
				motion.currentMotion = EMotion_Straight;
			} else if(currentAngle == E_AngleRight){
				motion.currentMotion = EMotion_Left90;
			} else if(currentAngle == E_AngleDown){
				motion.currentMotion = EMotion_Turn180;
			} else {
				motion.currentMotion = EMotion_Right90;
			}
		} else if(mapdata.isExistWall(currentCoordinate.x, currentCoordinate.y, E_AngleRight)==0 && footdata.getFootmap(currentCoordinate.x, currentCoordinate.y, 512) > footdata.getFootmap(currentCoordinate.x+1, currentCoordinate.y, 512) && (mode != 10 || mapdata.hasReached(currentCoordinate.x+1, currentCoordinate.y))){
			if(currentAngle == E_AngleUp){
				motion.currentMotion = EMotion_Right90;
			} else if(currentAngle == E_AngleRight){
				motion.currentMotion = EMotion_Straight;
			} else if(currentAngle == E_AngleDown){
				motion.currentMotion = EMotion_Left90;
			} else {
				motion.currentMotion = EMotion_Turn180;
			}
		} else if(mapdata.isExistWall(currentCoordinate.x, currentCoordinate.y, E_AngleDown)==0 && footdata.getFootmap(currentCoordinate.x, currentCoordinate.y, 512) > footdata.getFootmap(currentCoordinate.x, currentCoordinate.y-1, 512) && (mode != 10 || mapdata.hasReached(currentCoordinate.x, currentCoordinate.y-1))){
			if(currentAngle == E_AngleUp){
				motion.currentMotion = EMotion_Turn180;
			} else if(currentAngle == E_AngleRight){
				motion.currentMotion = EMotion_Right90;
			} else if(currentAngle == E_AngleDown){
				motion.currentMotion = EMotion_Straight;
			} else {
				motion.currentMotion = EMotion_Left90;
			}
		} else if(mapdata.isExistWall(currentCoordinate.x, currentCoordinate.y, E_AngleLeft)==0 && footdata.getFootmap(currentCoordinate.x, currentCoordinate.y, 512) > footdata.getFootmap(currentCoordinate.x-1, currentCoordinate.y, 512) && (mode != 10 || mapdata.hasReached(currentCoordinate.x-1, currentCoordinate.y))){
			if(currentAngle == E_AngleUp){
				motion.currentMotion = EMotion_Left90;
			} else if(currentAngle == E_AngleRight){
				motion.currentMotion = EMotion_Turn180;
			} else if(currentAngle == E_AngleDown){
				motion.currentMotion = EMotion_Right90;
			} else {
				motion.currentMotion = EMotion_Straight;
			}
		} else {
			motion.currentMotion = EMotion_null;
			Datalog::saveMaze(mapdata, 0);
		}
		if(motion.currentMotion == EMotion_Turn180 && mode == 1){
			motion.currentMotion = EMotion_Turn180_None;
		}
	}
	Position::setNextAngle(motion.currentMotion);
	return motion;
}

void Algorithm::adachiRenewFootmap(Triple<char> input, Footmap &foot, char is_first){
	Triple<char> buf;
	Triple<char> tmp;

	foot.resetFootmap();
	tmp.x = input.x;
	tmp.y = input.y;
	tmp.t = input.t + 1;
	que.reset();
	if(que.push(tmp)){
		Peripherals::addLED('R');
		return;
	}
	foot.setFootmap(input.x, input.y, input.t);
	while(que.empty() == 0){
		buf = que.pop();
		if(mapdata.isExistWall(buf.x, buf.y, E_AngleUp)==0 && foot.getFootmap(buf.x, buf.y+1, 0)==255 && (is_first ? (mapdata.hasReached(buf.x,buf.y+1)) : 1)){
			tmp.x = buf.x;
			tmp.y = buf.y+1;
			tmp.t = buf.t+1;
			que.push(tmp);
			foot.setFootmap(buf.x, buf.y+1, buf.t);
		}
		if(mapdata.isExistWall(buf.x, buf.y, E_AngleRight)==0 && foot.getFootmap(buf.x+1, buf.y, 0)==255 && (is_first ? (mapdata.hasReached(buf.x+1,buf.y)) : 1)){
			tmp.x = buf.x+1;
			tmp.y = buf.y;
			tmp.t = buf.t+1;
			que.push(tmp);
			foot.setFootmap(buf.x+1, buf.y, buf.t);
		}
		if(mapdata.isExistWall(buf.x, buf.y, E_AngleDown)==0 && foot.getFootmap(buf.x, buf.y-1, 0)==255 && (is_first ? (mapdata.hasReached(buf.x,buf.y-1)) : 1)){
			tmp.x = buf.x;
			tmp.y = buf.y-1;
			tmp.t = buf.t+1;
			que.push(tmp);
			foot.setFootmap(buf.x, buf.y-1, buf.t);
		}
		if(mapdata.isExistWall(buf.x, buf.y, E_AngleLeft)==0 && foot.getFootmap(buf.x-1, buf.y, 0)==255 && (is_first ? (mapdata.hasReached(buf.x-1,buf.y)) : 1)){
			tmp.x = buf.x-1;
			tmp.y = buf.y;
			tmp.t = buf.t+1;
			que.push(tmp);
			foot.setFootmap(buf.x-1, buf.y, buf.t);
		}
	}
}


int Algorithm::getPath(Path &p, float v, int mode){
	int c = 0;
	MouseMotion motion = {
			EMotion_null,
			0,
			0.5,
			EMotion_null
	};
	Position::setPosition(Pair<char>(0, 1), E_AngleUp);
	Triple<char> t = Triple<char>(goalCoordinate.x, goalCoordinate.y, 0);
	//	t.x = goalCoordinate.x;
	//	t.y = goalCoordinate.y;
	//	t.t = 0;
	Peripherals::setLED(' ');
	adachiRenewFootmap(t, footdata, 1);

	do{
		currentCoordinate = Position::getCoordinate();
		currentAngle = Position::getAngle();
		if(currentCoordinate.x == goalCoordinate.x && currentCoordinate.y == goalCoordinate.y){
			motion.currentMotion = EMotion_null;
		} else {
			if((mapdata.isExistWall(currentCoordinate.x, currentCoordinate.y, E_AngleUp)==0 && footdata.getFootmap(currentCoordinate.x, currentCoordinate.y, 512) > footdata.getFootmap(currentCoordinate.x, currentCoordinate.y+1, 512))){
				if(currentAngle == E_AngleUp){
					motion.currentMotion = EMotion_Straight;
				} else if(currentAngle == E_AngleRight){
					motion.currentMotion = EMotion_Left90;
				} else if(currentAngle == E_AngleDown){
					motion.currentMotion = EMotion_Turn180;
				} else {
					motion.currentMotion = EMotion_Right90;
				}
			} else if((mapdata.isExistWall(currentCoordinate.x, currentCoordinate.y, E_AngleRight)==0 && footdata.getFootmap(currentCoordinate.x, currentCoordinate.y, 512) > footdata.getFootmap(currentCoordinate.x+1, currentCoordinate.y, 512))){
				if(currentAngle == E_AngleUp){
					motion.currentMotion = EMotion_Right90;
				} else if(currentAngle == E_AngleRight){
					motion.currentMotion = EMotion_Straight;
				} else if(currentAngle == E_AngleDown){
					motion.currentMotion = EMotion_Left90;
				} else {
					motion.currentMotion = EMotion_Turn180;
				}
			} else if((mapdata.isExistWall(currentCoordinate.x, currentCoordinate.y, E_AngleDown)==0 && footdata.getFootmap(currentCoordinate.x, currentCoordinate.y, 512) > footdata.getFootmap(currentCoordinate.x, currentCoordinate.y-1, 512))){
				if(currentAngle == E_AngleUp){
					motion.currentMotion = EMotion_Turn180;
				} else if(currentAngle == E_AngleRight){
					motion.currentMotion = EMotion_Right90;
				} else if(currentAngle == E_AngleDown){
					motion.currentMotion = EMotion_Straight;
				} else {
					motion.currentMotion = EMotion_Left90;
				}
			} else if((mapdata.isExistWall(currentCoordinate.x, currentCoordinate.y, E_AngleLeft)==0 && footdata.getFootmap(currentCoordinate.x, currentCoordinate.y, 512) > footdata.getFootmap(currentCoordinate.x-1, currentCoordinate.y, 512))){
				if(currentAngle == E_AngleUp){
					motion.currentMotion = EMotion_Left90;
				} else if(currentAngle == E_AngleRight){
					motion.currentMotion = EMotion_Turn180;
				} else if(currentAngle == E_AngleDown){
					motion.currentMotion = EMotion_Right90;
				} else {
					motion.currentMotion = EMotion_Straight;
				}
			} else {
				motion.currentMotion = EMotion_null;
			}
		}
		Position::setNextAngle(motion.currentMotion);
		p.setNext(motion.currentMotion);
		++ c;
	} while(motion.currentMotion != EMotion_null && c < 128);
	Position::setPosition(Pair<char>(0, 1), E_AngleUp);
	p.resetPos();

	// 直進を半区間ベースに
	unsigned char zip_path[256];
	unsigned char ite_path = 0;
	for(int i=0; i<256; ++i){
		if(p.data[i] == EMotion_Straight){
			zip_path[ite_path++] = EMotion_Straight;
			zip_path[ite_path++] = EMotion_Straight;
		} else if(p.data[i] == EMotion_null || p.data[i] == EMotion_stop){
			zip_path[ite_path] = p.data[i];
			break;
		} else {
			zip_path[ite_path++] = p.data[i];
		}
	}
	ite_path = 0;
	for(int i=0; i<256; ++i){
		p.data[i] = zip_path[i];
		p.division[i] = 1;
	}

	if(mode == 0){
		unsigned char zip_division[256];
		unsigned char ite_straight_count = 0;
		ite_path = 0;
		ite_straight_count = 0;
		for(int i=0; i<256; ++i){
			if(p.data[i] == EMotion_Straight){
				++ ite_straight_count;
				if(i == 0) ++ ite_straight_count;
			} else {
				if(ite_straight_count){
					zip_path[ite_path] = EMotion_Straight;
					zip_division[ite_path++] = ite_straight_count;
					ite_straight_count = 0;
				}
				zip_path[ite_path++] = p.data[i];
				if(p.data[i] == EMotion_null) break;
			}
		}
		for(int i=0; i<256; ++i){
			p.data[i] = zip_path[i];
			p.division[i] = zip_division[i];
		}
		return ite_path+1;
	} else if(mode == 1){
		// 圧縮
		unsigned char zip_division[256];
		for(int i=0; i<256; ++i){
			zip_path[i] = 0;
			zip_division[i] = 1;
		}
		unsigned char ite_straight_count = 0;
		unsigned char ite_newpath = 0;
		for(ite_path=0; ite_path<256; ++ite_path){
			if(p.data[ite_path] == EMotion_null){
				zip_path[ite_newpath] = EMotion_null;
				break;
			} else if(p.data[ite_path] == EMotion_Straight){
				if(p.data[ite_path+1] == EMotion_Straight){
					++ ite_straight_count;
					if(ite_path == 0) ++ ite_straight_count;
				} else {
					if(ite_path == 0) ++ ite_straight_count;
					zip_path[ite_newpath] = EMotion_Straight;
					zip_division[ite_newpath++] = ite_straight_count;
					ite_straight_count = 0;
					if(p.data[ite_path+1] == EMotion_Right90){
						if(p.data[ite_path+2] == EMotion_Straight){
							zip_path[ite_newpath++] = EMotion_BigRight90;
							ite_path += 2;
						} else if(p.data[ite_path+2] == EMotion_Right90){
							if(p.data[ite_path+3] == EMotion_Straight){
								zip_path[ite_newpath++] = EMotion_BigRight180;
								ite_path += 2;
							} else {
								zip_path[ite_newpath++] = EMotion_Right90;
								zip_path[ite_newpath++] = EMotion_Left90;
								ite_path += 2;
							}
						} else if(p.data[ite_path+2] == EMotion_null){
							zip_path[ite_newpath++] = EMotion_BigRight90;
							zip_path[ite_newpath] = EMotion_null;
							break;
						} else {
							zip_path[ite_newpath++] = EMotion_Right90;
							zip_path[ite_newpath++] = EMotion_Left90;
						}
					} else if(p.data[ite_path+1] == EMotion_Left90){
						if(p.data[ite_path+2] == EMotion_Straight){
							zip_path[ite_newpath++] = EMotion_BigLeft90;
							ite_path += 2;
						} else if(p.data[ite_path+2] == EMotion_Left90){
							if(p.data[ite_path+3] == EMotion_Straight){
								zip_path[ite_newpath++] = EMotion_BigLeft180;
								ite_path += 2;
							} else {
								zip_path[ite_newpath++] = EMotion_Left90;
								zip_path[ite_newpath++] = EMotion_Right90;
								ite_path += 2;
							}
						} else if(p.data[ite_path+2] == EMotion_null){
							zip_path[ite_newpath++] = EMotion_BigLeft90;
							zip_path[ite_newpath] = EMotion_null;
							break;
						}
					} else if(p.data[ite_path+1] == EMotion_null || 1){
						zip_path[ite_newpath] = EMotion_Straight;
						zip_path[ite_newpath++] = 1;
						zip_path[ite_newpath] = EMotion_null;
						break;
					}
				}
			} else {
				zip_path[ite_newpath++] = p.data[ite_path];
			}
		}
		ite_path = 0;
		for(int i=0; i<256; ++i){
			if(zip_division[i] == 0 && zip_path[i] == EMotion_Straight){}
			else {
				p.data[ite_path] = zip_path[i];
				p.division[ite_path++] = zip_division[i];
			}
		}
		p.data[255] = EMotion_null;
		return ite_newpath+1;
	} else if(mode == 2){
		// 圧縮
		//	unsigned char zip_path[128];
		unsigned char zip_division[256];
		for(int i=0; i<256; ++i){
			zip_path[i] = 0;
			zip_division[i] = 1;
		}
		//	unsigned char ite_path = 0;
		unsigned char ite_straight_count = 0;
		unsigned char ite_newpath = 0;
		unsigned char is_current_oblique = 0;
		unsigned char last_oblique_direction = 0;	/// @brief 1:右，0:左
		for(ite_path=0; ite_path<256; ++ite_path){
			if(is_current_oblique){
				if(last_oblique_direction){ // 右。次左なら斜め継続
					if(p.data[ite_path] == EMotion_Left90){
						if(p.data[ite_path+1] == EMotion_Left90){
							if(p.data[ite_path+2] == EMotion_Straight){
								zip_path[ite_newpath++] = EMotion_Left135o;
								ite_path += 2;
								is_current_oblique = 0;
							} else if(p.data[ite_path+2] == EMotion_Right90){
								zip_path[ite_newpath++] = EMotion_ObliqueLeft90;
								ite_path += 1;
								last_oblique_direction = 0;
							} else if(p.data[ite_path+2] == EMotion_null) {
								zip_path[ite_newpath++] = EMotion_Left135o;
								ite_path += 1;
								is_current_oblique = 0;
								zip_path[ite_newpath] = EMotion_null;
								break;
							} else {
								/// @todo 例外処理
							}
						} else if(p.data[ite_path+1] == EMotion_Straight){
							zip_path[ite_newpath++] = EMotion_Left45o;
							ite_path += 1;
							is_current_oblique = 0;
						} else if(p.data[ite_path+1] == EMotion_Right90){
							zip_path[ite_newpath++] = EMotion_Oblique;
						} else if(p.data[ite_path+1] == EMotion_null) {
							zip_path[ite_newpath++] = EMotion_Left45o;
							ite_path += 1;
							is_current_oblique = 0;
							zip_path[ite_newpath] = EMotion_null;
							break;
						} else {
							/// @todo 例外処理
						}
					}
					last_oblique_direction = 0;
				} else { // 左。次右なら斜め継続
					if(p.data[ite_path] == EMotion_Right90){
						if(p.data[ite_path+1] == EMotion_Right90){
							if(p.data[ite_path+2] == EMotion_Straight){
								zip_path[ite_newpath++] = EMotion_Right135o;
								ite_path += 2;
								is_current_oblique = 0;
							} else if(p.data[ite_path+2] == EMotion_Left90){
								zip_path[ite_newpath++] = EMotion_ObliqueRight90;
								ite_path += 1;
								last_oblique_direction = 1;
							} else if(p.data[ite_path+2] == EMotion_null) {
								zip_path[ite_newpath++] = EMotion_Right45o;
								ite_path += 1;
								is_current_oblique = 0;
								zip_path[ite_newpath] = EMotion_null;
								break;
							} else {
								/// @todo 例外処理
							}
						} else if(p.data[ite_path+1] == EMotion_Straight){
							zip_path[ite_newpath++] = EMotion_Right45o;
							ite_path += 1;
							is_current_oblique = 0;
						} else if(p.data[ite_path+1] == EMotion_Left90){
							zip_path[ite_newpath++] = EMotion_Oblique;
						} else if(p.data[ite_path+1] == EMotion_null) {
							zip_path[ite_newpath++] = EMotion_Right45o;
							ite_path += 1;
							is_current_oblique = 0;
							zip_path[ite_newpath] = EMotion_null;
							break;
						} else {
							/// @todo 例外処理
						}
					}
					last_oblique_direction = 1;
				}
			} else {
				if(p.data[ite_path] == EMotion_null){
					zip_path[ite_newpath] = EMotion_null;
					break;
				} else if(p.data[ite_path] == EMotion_Straight){
					if(p.data[ite_path+1] == EMotion_Straight){
						++ ite_straight_count;
						if(ite_path == 0) ++ ite_straight_count;
					} else {
						if(ite_path == 0) ++ ite_straight_count;
						zip_path[ite_newpath] = EMotion_Straight;
						zip_division[ite_newpath++] = ite_straight_count;
						ite_straight_count = 0;
						if(p.data[ite_path+1] == EMotion_Right90){
							if(p.data[ite_path+2] == EMotion_Left90){
								/// @todo 直進区間をまとめる
								zip_path[ite_newpath] = EMotion_Right45i;
								++ ite_newpath;
								ite_path += 1;
								is_current_oblique = 1;
								last_oblique_direction = 1;
							} else if(p.data[ite_path+2] == EMotion_Straight){
								zip_path[ite_newpath++] = EMotion_BigRight90;
								ite_path += 2;
							} else if(p.data[ite_path+2] == EMotion_Right90){
								if(p.data[ite_path+3] == EMotion_Straight){
									zip_path[ite_newpath++] = EMotion_BigRight180;
									ite_path += 3;
								} else { // EMotion_Left90
									zip_path[ite_newpath++] = EMotion_Right135i;
									ite_path += 2;
									is_current_oblique = 1;
									last_oblique_direction = 1;
								}
							} else if(p.data[ite_path+2] == EMotion_null){
								zip_path[ite_newpath++] = EMotion_BigRight90;
								zip_path[ite_newpath] = EMotion_null;
								break;
							}
						} else if(p.data[ite_path+1] == EMotion_Left90){
							if(p.data[ite_path+2] == EMotion_Right90){
								/// @todo 直進区間をまとめる
								zip_path[ite_newpath] = EMotion_Left45i;
								++ ite_newpath;
								ite_path += 1;
								is_current_oblique = 1;
								last_oblique_direction = 0;
							} else if(p.data[ite_path+2] == EMotion_Straight){
								zip_path[ite_newpath++] = EMotion_BigLeft90;
								ite_path += 2;
							} else if(p.data[ite_path+2] == EMotion_Left90){
								if(p.data[ite_path+3] == EMotion_Straight){
									zip_path[ite_newpath++] = EMotion_BigLeft180;
									ite_path += 3;
								} else { // EMotion_Right90
									zip_path[ite_newpath++] = EMotion_Left135i;
									ite_path += 2;
									is_current_oblique = 1;
									last_oblique_direction = 0;
								}
							} else if(p.data[ite_path+2] == EMotion_null){
								zip_path[ite_newpath++] = EMotion_BigLeft90;
								zip_path[ite_newpath] = EMotion_null;
								break;
							}
						} else if(p.data[ite_path+1] == EMotion_null || 1){
							zip_path[ite_newpath] = EMotion_Straight;
							zip_path[ite_newpath++] = 1;
							zip_path[ite_newpath] = EMotion_null;
							break;
						}
					}
				} else if(p.data[ite_path] == EMotion_Right90){
					if(p.data[ite_path+1] == EMotion_Right90){
						zip_path[ite_newpath++] = EMotion_Right135i;
						last_oblique_direction = 1;
						is_current_oblique = 1;
						ite_path += 1;
					} else if(p.data[ite_path+1] == EMotion_Straight){
						zip_path[ite_newpath++] = EMotion_Right90;
					} else if(p.data[ite_path+1] == EMotion_Left90){
						zip_path[ite_newpath++] = EMotion_Right45i;
						last_oblique_direction = 1;
						is_current_oblique = 1;
						//					ite_path += 1;
					}
				} else {
					/// @todo 右ターンスタートの処理を改善
					/// @todo 最短走行時のスタート処理を改善
					p.data[ite_path] == EMotion_null;
					break;
				}
			}
		}

		//	// 斜め直線を圧縮
		//	for(int i=0; i<256; ++i){
		//		p.data[i] = zip_path[i];
		//		p.division[i] = zip_division[i];
		//	}
		//	ite_path = 0;
		//	ite_straight_count = 0;
		//	for(int i=0; i<256; ++i){
		//		if(p.data[i] == EMotion_Oblique){
		//			++ ite_straight_count;
		//		} else if(p.data[i] == EMotion_null){
		//			zip_path[ite_path] = p.data[i];
		//			break;
		//		} else {
		//			zip_path[ite_path++] = p.data[i];
		//		}
		//	}
		//	for(int i=0; i<256; ++i) p.data[i] = zip_path[i];

		ite_path = 0;
		for(int i=0; i<256; ++i){
			if(zip_division[i] == 0 && zip_path[i] == EMotion_Straight){}
			else {
				p.data[ite_path] = zip_path[i];
				p.division[ite_path++] = zip_division[i];
			}
		}
		p.data[255] = EMotion_null;

		return ite_newpath+1;
		//	return c;
	}
}




