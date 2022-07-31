//
// Created by q8239 on 2022/7/31.
//

#ifndef _MAP_CALIBRATION_ENCODER_SENSOR_H
#define _MAP_CALIBRATION_ENCODER_SENSOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CIRCLE          1440
#define POSITION_TO_MM  2.67405e-10f

extern int32_t left_encoder;
extern int32_t right_encoder;

extern int64_t encoder_x;
extern int64_t encoder_y;

void encoder_sensor_init(void);

float encoder_get_angle(void);

void encoder_get_position(float *x, float *y);

#ifdef __cplusplus
}
#endif

#endif //_MAP_CALIBRATION_ENCODER_SENSOR_H
