//
// Created by q8239 on 2022/7/31.
//

#include "map_calibration.h"

#include <stdlib.h>
#include <math.h>

# define PI_2_FLOAT32 (2 * 3.14159265358979323846f)

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

bool map_calibration_init(map_calibration_t *self, size_t buffer_size) {
    self->rotation = 0;
    self->sensor_center = (float32_2_t) {0, 0};
    self->world_center = (float32_2_t) {0, 0};
    self->buffer_size = buffer_size;
    self->buffer_count = 0;
    self->current_index = 0;
    if (buffer_size < MAP_CALIBRATION_MIN_COUNT) {
        return true;
    }
    self->buffer = malloc(sizeof(map_calibration_point_t) * buffer_size);
    return self->buffer == NULL;
}

void map_calibration_uninit(map_calibration_t *self) {
    free(self->buffer);
}

void map_calibration_reset(map_calibration_t *self) {
    self->rotation = 0;
    self->sensor_center = (float32_2_t) {0, 0};
    self->world_center = (float32_2_t) {0, 0};
    self->buffer_count = 0;
    self->current_index = 0;
}

void map_calibration_transform(map_calibration_t *self, float32_2_t sensor_coord, float32_2_t *world_coord) {
    float32_t x = sensor_coord.x - self->sensor_center.x;
    float32_t y = sensor_coord.y - self->sensor_center.y;
    float32_t sinR = sinf(self->rotation);
    float32_t cosR = cosf(self->rotation);
    world_coord->x = x * cosR - y * sinR + self->world_center.x;
    world_coord->y = x * sinR + y * cosR - self->world_center.y;
}

void map_calibration_point(map_calibration_t *self, float32_2_t sensor_coord, float32_2_t world_coord) {
    map_calibration_point_t *buffer = self->buffer;
    buffer[self->current_index++] = (map_calibration_point_t) {sensor_coord, world_coord};
    if (self->current_index == self->buffer_size) {
        self->current_index = 0;
    }
    if (self->buffer_count < self->buffer_size) {
        self->buffer_count++;
    }

    size_t size = self->buffer_size;
    size_t index = self->current_index;
    size_t count = self->buffer_count;
    float32_t inv_count = 1.0f / count;
    float32_2_t sensor_sum = {0, 0};
    float32_2_t world_sum = {0, 0};
    for (size_t i = 0; i < count; i++) {
        const map_calibration_point_t *map_point = &buffer[(index + i + size - count) % size];
        sensor_sum.x += map_point->sensor_coord.x;
        sensor_sum.y += map_point->sensor_coord.y;
        world_sum.x += map_point->world_coord.x;
        world_sum.y += map_point->world_coord.y;
    }
    float32_2_t sensor_center = (float32_2_t) {sensor_sum.x * inv_count, sensor_sum.y * inv_count};
    float32_2_t world_center = (float32_2_t) {world_sum.x * inv_count, world_sum.y * inv_count};
    self->sensor_center = sensor_center;
    self->world_center = world_center;

    if (count >= MAP_CALIBRATION_MIN_COUNT) {
        float32_t rotation_sum = 0;
        float32_t power_sum = 0;
        for (size_t i = 0; i < count; i++) {
            const map_calibration_point_t *map_point = &buffer[(index + i + size - count) % size];
            float32_2_t sensor_center_coord = (float32_2_t) {
                map_point->sensor_coord.x - sensor_center.x,
                map_point->sensor_coord.y - sensor_center.y,
            };
            float32_2_t world_center_coord = (float32_2_t) {
                map_point->world_coord.x - world_center.x,
                map_point->world_coord.y - world_center.y,
            };
            float32_t power_sensor = sqrtf(sensor_center_coord.x * sensor_center_coord.x + sensor_center_coord.y * sensor_center_coord.y);
            float32_t power_world = sqrtf(world_center_coord.x * world_center_coord.x + world_center_coord.y * world_center_coord.y);
            float32_t power = min(power_sensor, power_world);
            power_sum += power;
            float32_t sensor_angle = atan2f(sensor_center_coord.y, sensor_center_coord.x);
            float32_t world_angle = atan2f(world_center_coord.y, world_center_coord.x);
            float32_t delta_rotation = world_angle - sensor_angle - rotation_sum;
            delta_rotation -= roundf(delta_rotation / PI_2_FLOAT32) * PI_2_FLOAT32;
            rotation_sum += delta_rotation * power;
        }
        self->rotation = rotation_sum / power_sum;
    }
}

bool map_calibration_point_from_sensor_and_map(map_calibration_t *self, const near_map_t *map, float32_2_t sensor_coord, float32_t sensor_angle, float32_2_t relative) {
    float32_2_t center_world_coord;
    map_calibration_transform(self, sensor_coord, &center_world_coord);
    float32_2_t world_relative;
    float32_t angle = sensor_angle + self->rotation;
    float32_t sinR = sinf(angle);
    float32_t cosR = cosf(angle);
    float32_2_t test = {
        center_world_coord.x + relative.x * cosR - relative.y * sinR,
        center_world_coord.y + relative.x * sinR + relative.y * cosR,
    };
    if (map->get_nearest(map, test, &world_relative)) {
        return true;
    }
    map_calibration_point(self, test, world_relative);
    return false;
}
