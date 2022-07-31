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

typedef struct map_calibration_point {
    float32_2_t sensor_coord;
    float32_2_t world_coord;
} map_calibration_point_t;

/// 地图校正对象
typedef struct map_calibration {
    /// 角度误差
    /// 编码器角度加上此值为世界角度
    float32_t rotation;
    /// 校准点在传感坐标空间中的重心
    float32_2_t sensor_center;
    /// 校准点在世界坐标空间中的重心
    float32_2_t world_center;
    /// 用于储存校准点的缓冲区
    map_calibration_point_t *buffer;
    /// 可以储存校准点的最大数量
    size_t buffer_size;
    /// 当前储存了的校准点数量
    size_t buffer_count;
    /// 下一个储存校准点的位置
    size_t current_index;
} map_calibration_t;

/// 初始化地图校正对象，为校准点缓冲分配存储空间
///
/// \param self         地图校正对象
/// \param buffer_size  储存的最大校准点数
/// \return             初始化是否出现错误
bool map_calibration_init(map_calibration_t *self, size_t buffer_size);

/// 反初始化地图校正对象，释放校准点缓冲的存储空间
///
/// \param self         地图校正对象
void map_calibration_uninit(map_calibration_t *self);

/// 复位地图校正对象，这将会清空当前的所有采样点
///
/// \param self         地图校正对象
/// \param sensor_coord 传感器默认位置
/// \param world_coord  地图默认位置
/// \param rotation     默认旋转角度
void map_calibration_reset(map_calibration_t *self, float32_2_t sensor_coord, float32_2_t world_coord, float32_t rotation);

/// 利用地图校正对象完成坐标变换
///
/// \param self         地图校正对象
/// \param sensor_coord 编码器坐标
/// \param world_coord  世界坐标
void map_calibration_transform(map_calibration_t *self, float32_2_t sensor_coord, float32_2_t *world_coord);

/// 增加地图校正点
///
/// \param self         地图校正对象
/// \param sensor_coord 编码器坐标
/// \param world_coord  世界坐标
void map_calibration_point(map_calibration_t *self, float32_2_t sensor_coord, float32_2_t world_coord);

/// 从最临近地图中计算出相对于当前位置指定偏移的点的世界坐标，从而增加一个校准
///
/// \param self         地图校正对象
/// \param map          最邻近地图
/// \param sensor_coord 编码器坐标
/// \param sensor_angle 编码器方向
/// \param relative     检测到地标的位置，相对于物体，以前方为 X 轴，左侧为 Y 轴
/// \return
bool map_calibration_point_from_sensor_and_map(map_calibration_t *self, const near_map_t *map, float32_2_t sensor_coord, float32_t sensor_angle, float32_2_t relative);

#ifdef __cplusplus
}
#endif

#endif //MAP_CALIBRATION_H
