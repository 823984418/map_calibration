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

/// 最临近地图对象
typedef struct near_map {
    /// 用户数据
    void *user_data;
    /// 获取某个点的最邻近点
    bool (*get_nearest)(const struct near_map *self, float32_2_t pos, float32_2_t *near);

} near_map_t;

/// 适用于2022年浙江省电赛小车题的最邻近地图
extern const near_map_t ds2022_map;

#ifdef __cplusplus
}
#endif

#endif //_MAP_CALIBRATION_NEAR_MAP_H
