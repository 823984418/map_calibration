//
// Created by q8239 on 2022/7/31.
//

#ifndef MAP_CALIBRATION_H
#define MAP_CALIBRATION_H

#include <stdbool.h>
#include <stddef.h>

#include "near_map.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAP_CALIBRATION_MIN_COUNT   8

typedef struct map_calibration {
    float32_t rotation;
    float32_2_t sensor_center;
    float32_2_t world_center;
    void *buffer;
    size_t buffer_size;
    size_t buffer_count;
    size_t current_index;
} map_calibration_t;

bool map_calibration_init(map_calibration_t *self, size_t buffer_size);

void map_calibration_uninit(map_calibration_t *self);

void map_calibration_reset(map_calibration_t *self);

void map_calibration_transform(map_calibration_t *self, float32_2_t sensor_coord, float32_2_t *world_coord);

void map_calibration_point(map_calibration_t *self, float32_2_t sensor_coord, float32_2_t world_coord);

bool map_calibration_point_from_sensor_and_map(map_calibration_t *self, const near_map_t *map, float32_2_t sensor_coord, float32_t sensor_angle, float32_2_t relative);

#ifdef __cplusplus
}
#endif

#endif //MAP_CALIBRATION_H
