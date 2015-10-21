/**
 * @file params_include.h
 * センサや機体の調整パラメータをまとめたファイルです
 */
#ifndef INCLUDED_PARAMSINCLUDE_H
#define INCLUDED_PARAMSINCLUDE_H

/**************************************/
/*  機体設定                          */
/**************************************/

//タイヤ直径(mm)
extern const float TIRE_R;

//タイヤ円周(mm)
extern const float TIRE_ROUND;

//タイヤ1周分のエンコーダパルス数
extern const int TIRE_ROUND_PULSE;

//エンコーダ1パルスあたりの距離(m)
extern const float DISTANCE_PULSE;


/**************************************/
/*  センサー設定                      */
/**************************************/

//ジャイロの感度(要詳細)
extern const float SENSITIV_GYRO_R;
extern const float SENSITIV_GYRO_L;

//センサー閾値
extern const int SENSOR_LIM_FL;
extern const int SENSOR_LIM_L;
extern const int SENSOR_LIM_R;
extern const int SENSOR_LIM_FR;

//センサーリファレンス値
extern const int SENSOR_REF_FL;
extern const int SENSOR_REF_L;
extern const int SENSOR_REF_R;
extern const int SENSOR_REF_FR;


/**************************************/
/*  モーター設定                      */
/**************************************/

//Uncomment to MAXSPEED MODE
#define SPEEDLIMIT_MODE

//慣性モーメント
extern const float MOMENT_INERTIA;

//機体重量(kg)
extern const float WEIGHT;

//トレッド幅(m)
extern const float TREAD;

//FF制御時のK_pゲイン
extern const float GAIN_IDEAL;

//壁ゲインK_p
extern const float GAIN_KABE_R;
extern const float GAIN_KABE_L;

//壁ゲインK_i
extern const float GAIN_KABE_INTEGRAL;

//FB制御のエンコーダK_pゲイン
extern const float GAIN_ENCODER_R;
extern const float GAIN_ENCODER_L;

//FB制御のエンコーダK_iゲイン
extern const float GAIN_INTEGRAL_R;
extern const float GAIN_INTEGRAL_L;

//FB制御のエンコーダ角速度成分のK_pゲイン
extern const float GAIN_RAD;

//FB制御のジャイロK_pゲイン
extern const float GAIN_GYRO_R;
extern const float GAIN_GYRO_L;

//FB制御のジャイロK_iゲイン
extern const float GAIN_GYRO_INTEGRAL_R;
extern const float GAIN_GYRO_INTEGRAL_L;

//デフォルトの直進加速度
extern const float ACCEL;

//デフォルトの角加速度
extern const float RADACCEL;


#endif
