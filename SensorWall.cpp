
#include "SensorWall.hpp"


float SensorWall::current_value[4] = {0.0, 0.0, 0.0, 0.0};
char SensorWall::is_working = 0;
// int SensorWall::ref_straight_value[4] = {SENSOR_REF_FL, SENSOR_REF_L, SENSOR_REF_R, SENSOR_REF_FR};
// int SensorWall::thr_straight_value[4] = {SENSOR_LIM_FL, SENSOR_LIM_L, SENSOR_LIM_R, SENSOR_LIM_FR};
int SensorWall::ref_straight_value[4] = {0, 7, 49, 0};
int SensorWall::thr_straight_value[4] = {100, 120, 210, 100};


void SensorWall::initialize(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	ADC_Cmd(ADC1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC|RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);
	ADC_DeInit();
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1,DISABLE);
}

void SensorWall::onLed(){
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
}

void SensorWall::offLed(){
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
}

void SensorWall::start(){
	is_working = 1;
}

void SensorWall::stop(){
	is_working = 0;
}

void SensorWall::interrupt(){
	if(is_working == 0) return;
	
	int tmp[4] = {0, 0, 0, 0};

	onLed();
	// for(int i=0; i<400; ++i);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	tmp[E_LFront] = ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	tmp[E_Left] = ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	tmp[E_Right] = ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	tmp[E_RFront] = ADC_GetConversionValue(ADC1);

	current_value[0] = tmp[0];
	current_value[1] = tmp[1];
	current_value[2] = tmp[2];
	current_value[3] = tmp[3];
	return;

	offLed();
	for(int i=0; i<400; ++i);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	current_value[E_LFront] = tmp[E_LFront] - ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	current_value[E_Left] = tmp[E_Left] - ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	current_value[E_Right] = tmp[E_Right] - ADC_GetConversionValue(ADC1);

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	current_value[E_RFront] = tmp[E_RFront] - ADC_GetConversionValue(ADC1);

	onLed();
}


int SensorWall::getValue(ESensorPosition pos){
	return current_value[pos];
}

int SensorWall::existWall(ESensorPosition pos){
	if(current_value[pos] > thr_straight_value[pos]) return 1;
	else return 0;
}

Walldata SensorWall::getWall(){}

int SensorWall::getCorrection(int max){
	int tmp_r;
	int tmp_l;

	tmp_r = getValue(E_Right) - ref_straight_value[E_MotorRight];
	tmp_l = ref_straight_value[E_MotorLeft] - getValue(E_Left);

	// if(existWall(E_LFront) != 0 && existWall(E_RFront) != 0) return 0;

	// if(tmpR < -50){
	// 	tmpR = 0;
	// 	tmpL *= 2;
	// }
	// if(tmpL < -50){
	// 	tmpL = 0;
	// 	tmpR *= 2;
	// }

	int retval = tmp_r + tmp_l;
	// if(existWall(E_Left)){
	// 	retval = tmpL;
	// } else if(existWall(E_Right)){
	// 	retval = tmpR;
	// } else {
	// 	retval = 0;
	// }

	if(retval > max) return max;
	if(-1*retval < -1*max) return -max;
	return retval;
}
