//
// Created by q8239 on 2022/7/31.
//

#ifndef _MAP_CALIBRATION_VEC_2_H
#define _MAP_CALIBRATION_VEC_2_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef float float32_t;
typedef double float64_t;

typedef struct int16_2 {
    int16_t x;
    int16_t y;
} int16_2_t;

typedef struct int32_2 {
    int32_t x;
    int32_t y;
} int32_2_t;

typedef struct int64_2 {
    int32_t x;
    int32_t y;
} int64_2_t;

typedef struct float32_2 {
    float32_t x;
    float32_t y;
} float32_2_t;

typedef struct float64_2 {
    float64_t x;
    float64_t y;
} float64_2_t;

#ifdef __cplusplus
}
#endif

#endif //_MAP_CALIBRATION_VEC_2_H
