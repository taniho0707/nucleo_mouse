/**
 * @file parameters.hpp
 * センサや機体の調整パラメータをまとめたファイルです
 */
#ifndef INCLUDED_PARAMETERS_H
#define INCLUDED_PARAMETERS_H

/**************************************/
/*  機体設定                          */
/**************************************/

//タイヤ直径(mm)
#define TIRE_R (17.061)

//タイヤ円周(mm)
#define TIRE_ROUND (53.6)//(47.125)

//タイヤ1周分のエンコーダパルス数
#define TIRE_ROUND_PULSE (200.0)

//1パルスあたりの距離(mm)
#define DISTANCE_PULSE (TIRE_ROUND/TIRE_ROUND_PULSE)


/**************************************/
/*  センサー設定                      */
/**************************************/

//センサー閾値
#define SENSOR_LIM_FL 1020
#define SENSOR_LIM_L 750
#define SENSOR_LIM_R 750
#define SENSOR_LIM_FR 910

//センサーリファレンス値
#define SENSOR_REF_FL 2580
#define SENSOR_REF_L 1385
#define SENSOR_REF_R 1390
#define SENSOR_REF_FR 2360

#define SENSOR_OBL_REF_L 890
#define SENSOR_OBL_REF_R 780


/**************************************/
/*  モーター設定                      */
/**************************************/

//前壁or壁当て 0か1
#define ENABLED_MAEKABE 0
#define ENABLED_KABEATE 1

//慣性モーメント
#define MOMENT_INERTIA 0.2

//機体重量(kg)
#define WEIGHT 0.100

//トレッド幅(mm)
#define TREAD (24.0)

//FF制御時のK_pゲイン
#define GAIN_IDEAL 0

//壁補正の最大値
#define MAX_WALL_CORRECTION 1000//300

//壁ゲインK_p
#define GAIN_KABE_R 0.01//0.006
#define GAIN_KABE_L 0.01//0.006
#define GAIN_KABE 0.007//0.01	//竹本制御の時は2，今井制御の時は0.01

//壁ゲインK_i
#define GAIN_KABE_INTEGRAL 0.05

//FB制御のエンコーダK_pゲイン
#define GAIN_ENCODER_R 100
#define GAIN_ENCODER_L 100
#define GAIN_ENCODER 100

//FB制御のエンコーダK_iゲイン
#define GAIN_INTEGRAL_R 230
#define GAIN_INTEGRAL_L 230
#define GAIN_INTEGRAL 200

//FB制御のエンコーダ角速度成分のK_pゲイン
#define GAIN_RAD 0

//FB制御のジャイロK_pゲイン
#define GAIN_GYRO_R 30//0
#define GAIN_GYRO_L 30//0
#define GAIN_GYRO 30//80//60

//FB制御のジャイロK_iゲイン
#define GAIN_GYRO_INTEGRAL_R 0.5//.05//3
#define GAIN_GYRO_INTEGRAL_L 0.5//.05//3
#define GAIN_GYRO_INTEGRAL 5.5//4//2	//竹本制御の時は0.2，今井制御の時は2

//デフォルトの直進加速度
#define ACCEL 6//1.5

//デフォルトの角加速度
#define RADACCEL 25


#endif
