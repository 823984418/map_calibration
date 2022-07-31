//
// Created by q8239 on 2022/7/31.
//

#ifndef _MAP_CALIBRATION_ENCODER_SENSOR_H
#define _MAP_CALIBRATION_ENCODER_SENSOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// 左侧编码器计数值
extern int32_t left_encoder;
/// 右侧编码器计数值
extern int32_t right_encoder;

/// 编码器坐标空间 X
extern int64_t encoder_x;
/// 编码器坐标空间 Y
extern int64_t encoder_y;

/// 初始化编码坐标传感器
void encoder_sensor_init(void);

/// 获取当前编码器坐标传感器角度信息
float encoder_get_angle(void);

/// 获取当前编码器坐标传感器坐标信息
void encoder_get_position(float *x, float *y);

#ifdef __cplusplus
}
#endif

#endif //_MAP_CALIBRATION_ENCODER_SENSOR_H
