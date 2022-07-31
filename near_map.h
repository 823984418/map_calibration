//
// Created by q8239 on 2022/7/31.
//

#ifndef _MAP_CALIBRATION_NEAR_MAP_H
#define _MAP_CALIBRATION_NEAR_MAP_H

#include <stdbool.h>

#include "vec_2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct near_map {
    void *user_data;

    bool (*get_nearest)(const struct near_map* self, float32_2_t pos, float32_2_t *near);

} near_map_t;

extern const near_map_t ds2022_map;

#ifdef __cplusplus
}
#endif

#endif //_MAP_CALIBRATION_NEAR_MAP_H
