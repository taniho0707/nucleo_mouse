/**
 * @file Motor.cpp
 */

#include "Motor.hpp"

TIM_TimeBaseInitTypeDef Motor::TIM_initstr;
NVIC_InitTypeDef Motor::NVIC_initstr;
TIM_OCInitTypeDef Motor::TIM_OC_initstr;
GPIO_InitTypeDef Motor::GPIO_initstr;

float Motor::target_velocity[2] = {0.0, 0.0};
int Motor::count_of_clear[2] = {1000, 1000};
float Motor::target_accel_gravity = 0;
float Motor::target_velocity_gravity = 0;

float Motor::total_distance[2] = {0.0, 0.0};

char Motor::is_current_forward[2] = {1, 1};
char Motor::is_working = 0;


void Motor::initialize(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_initstr.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_7;
	GPIO_initstr.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_initstr.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_initstr.GPIO_OType = GPIO_OType_PP;
	GPIO_initstr.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_Init(GPIOA, &GPIO_initstr);
	GPIO_initstr.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_initstr);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_initstr.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_6;
	GPIO_initstr.GPIO_Mode = GPIO_Mode_AF;
	GPIO_initstr.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_initstr.GPIO_OType = GPIO_OType_PP;
	GPIO_initstr.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_initstr);

	TIM_initstr.TIM_Period = 420-1;
	TIM_initstr.TIM_Prescaler = 200-1;
	TIM_initstr.TIM_ClockDivision = 0;
	TIM_initstr.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_initstr.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_initstr);
	TIM_TimeBaseInit(TIM3, &TIM_initstr);

	TIM_OC_initstr.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_initstr.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC_initstr.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_initstr.TIM_Pulse = 210-1;
	TIM_OC1Init(TIM2,&TIM_OC_initstr);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC1Init(TIM3,&TIM_OC_initstr);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);

	NVIC_initstr.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_initstr.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_initstr.NVIC_IRQChannelSubPriority = 1;
	NVIC_initstr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_initstr);
	NVIC_initstr.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_Init(&NVIC_initstr);

	TIM_TimeBaseInit(TIM2, &TIM_initstr);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_TimeBaseInit(TIM3, &TIM_initstr);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	
	stopRotate();

	disexcitate();
}

void Motor::excitate(){
	GPIO_SetBits(GPIOA, GPIO_Pin_10);
}

void Motor::disexcitate(){
	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
}

void Motor::startRotate(){
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	is_working = 1;
}

void Motor::stopRotate(){
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	is_working = 0;
}

void Motor::changeToForward(EMotorPosition side){
	if(side == E_MotorLeft)
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
	else
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
	is_current_forward[side] = 1;
}

void Motor::changeToBackward(EMotorPosition side){
	if(side == E_MotorLeft)
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
	else
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	is_current_forward[side] = 0;
}

void Motor::setSpeed(EMotorPosition side, float vel){
	float velocity;
	if(vel > 0){
		changeToForward(side);
		velocity = vel;
	} else {
		changeToBackward(side);
		velocity = -1 * vel;
	}
	
	count_of_clear[side] = static_cast<int>(12506667.0/velocity*2);
	TIM_initstr.TIM_Period = count_of_clear[side]/12000 - 1;
	TIM_initstr.TIM_Prescaler = 12000 - 1;
	TIM_initstr.TIM_ClockDivision = 0;
	TIM_initstr.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_initstr.TIM_RepetitionCounter = 0;

	TIM_OC_initstr.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_initstr.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC_initstr.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_initstr.TIM_Pulse = static_cast<int>(TIM_initstr.TIM_Period / 2);

	if(side == E_MotorLeft){
		TIM_TimeBaseInit(TIM3, &TIM_initstr);
		TIM_OC1Init(TIM3,&TIM_OC_initstr);
		TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	} else {
		TIM_TimeBaseInit(TIM2, &TIM_initstr);
		TIM_OC1Init(TIM2,&TIM_OC_initstr);
		TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	}
}

int Motor::interrupt(){
	if(is_working){
		setSpeed(E_MotorLeft, target_velocity[E_MotorLeft]);
		setSpeed(E_MotorRight, target_velocity[E_MotorRight]);
		// setSpeed(E_MotorLeft, target_velocity[E_MotorLeft]-0.8*SensorWall::getCorrection(100));
		// setSpeed(E_MotorRight, target_velocity[E_MotorRight]+0.8*SensorWall::getCorrection(100));
	}
}


void Motor::setLinAccel(float a){
	
}

void Motor::setLinVelocity(float v){
	
}


void Motor::setRadAccel(float a){
	
}

void Motor::setRadVelocity(float v){
	
}



void Motor::changeToOblique(){
	
}

void Motor::changeToStraight(){
	
}

unsigned char Motor::isCurrentOblique(){
	
}

void Motor::resetTotalDistance(){
	total_distance[E_MotorLeft] = 0.0;
	total_distance[E_MotorRight] = 0.0;
}

float Motor::getTotalDistanceGravity(){
	return (total_distance[E_MotorLeft]+total_distance[E_MotorRight])/2.0;
}

float Motor::getTotalDistanceAngle(){
	float tmp = 0;
	tmp = total_distance[E_MotorLeft] - total_distance[E_MotorRight];
	if(tmp < 0) tmp *= -1;
	return tmp*57.295/28.08;
	// return tmp*57.295/TREAD;
}

float Motor::addTotalDistance(float t, EMotorPosition side){
	if(is_current_forward[side])
		total_distance[side] += t;
	else
		total_distance[side] -= t;
	return total_distance[side];
}

void Motor::pulseRun(float max_vel, float distance){
	resetTotalDistance();
	setSpeed(E_MotorLeft, max_vel);
	setSpeed(E_MotorRight, max_vel);
	target_velocity[E_MotorLeft] = max_vel;
	target_velocity[E_MotorRight] = max_vel;
	startRotate();
	while(distance > getTotalDistanceGravity()){
		// setSpeed(E_MotorLeft, max_vel+GAIN_KABE_L*SensorWall::getCorrection(100));
		// setSpeed(E_MotorRight, max_vel-GAIN_KABE_R*SensorWall::getCorrection(100));
		Timer::wait_ms(1);
		// Usart::printf("%f\n", getTotalDistanceGravity());
		// Usart::printf("%d, %d, %d, %d\n", SensorWall::getCorrection(100), 0, SensorWall::getValue(E_Left), SensorWall::getValue(E_Right));
	}
	// Usart::printf("%f\n", getTotalDistanceGravity());
	target_velocity[E_MotorLeft] = 0;
	target_velocity[E_MotorRight] = 0;
	stopRotate();
	return;
}

void Motor::pulseTurn(float max_vel_deg, float distance){
	resetTotalDistance();
	setSpeed(E_MotorLeft, -1*max_vel_deg);
	setSpeed(E_MotorRight, max_vel_deg);
	target_velocity[E_MotorLeft] = -1*max_vel_deg;
	target_velocity[E_MotorRight] = max_vel_deg;
	startRotate();
	while(distance > getTotalDistanceAngle()){
		Timer::wait_ms(1);
		// Usart::printf("%f\n", getTotalDistanceAngle());
	}
	// Usart::printf("%f\n", getTotalDistanceAngle());
	target_velocity[E_MotorLeft] = 0;
	target_velocity[E_MotorRight] = 0;
	stopRotate();
	return;
}


float Motor::setLinBothendsVelocity(float startvl, float endvl, float distance){
	
}

float Motor::setRadBothendsVelocity(float startvl, float endvl, float degree){
	
}


void Motor::linTrapezoid(float distance, float startvl, float endvl, float maxSpeed){
	
}

void Motor::pivotturn(float deg){
	
}


void Motor::slalom(int mode, float v){
	
}



