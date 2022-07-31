//
// Created by q8239 on 2022/7/31.
//

#include "near_map.h"

#include <math.h>
#include <stddef.h>

#define MIN_DELTA   0.0001f

static void circle_shadow300(float32_2_t coord, float32_2_t *shadow, float32_t *dest) {
    float32_t len = sqrtf(coord.x * coord.x + coord.y * coord.y + MIN_DELTA);
    float32_t inv = 300.0f / len;
    *shadow = (float32_2_t) {coord.x * inv, coord.y * inv};
    if (dest) {
        *dest = len;
    }
}

static bool ds2022_map_nearest(const struct near_map *self, float32_2_t coord, float32_2_t *near) {
    float32_t x = coord.x;
    float32_t y = coord.y;
    float32_2_t shadow;
    float32_t len;
    if (x < 300) {
        if (y < 300) {
            circle_shadow300((float32_2_t) {x - 300, y - 300}, &shadow, NULL);
            *near = (float32_2_t) {shadow.x + 300, shadow.y + 300};
        } else if (y < 900) {
            *near = (float32_2_t) {0, y};
        } else {
            circle_shadow300((float32_2_t) {x - 300, y - 900}, &shadow, NULL);
            *near = (float32_2_t) {shadow.x + 300, shadow.y + 900};
        }
    } else if (x < 1200) {
        if (y < 600) {
            *near = (float32_2_t) {x, 0};
        } else {
            *near = (float32_2_t) {x, 1200};
        }
    } else if (x < 1650) {
        if (y < 300) {
            circle_shadow300((float32_2_t) {x - 1200, y - 300}, &shadow, &len);
            if (len - 300 > y) {
                *near = (float32_2_t) {x, 0};
            }
            *near = (float32_2_t) {shadow.x + 1200, shadow.y + 300};
            return true;
        } else if (y < 900) {
            *near = (float32_2_t) {1500, y};
        } else {
            circle_shadow300((float32_2_t) {x - 1200, y - 300}, &shadow, &len);
            if (len - 300 > 1200 - y) {
                *near = (float32_2_t) {x, 1200};
            }
            *near = (float32_2_t) {shadow.x + 1200, shadow.y + 900};
            return true;
        }
    } else {
        if (y < 300) {
            circle_shadow300((float32_2_t) {x - 1500, y - 300}, &shadow, NULL);
            *near = (float32_2_t) {shadow.x + 1500, shadow.y + 300};
        } else if (y < 900) {
            *near = (float32_2_t) {1800, y};
        } else {
            circle_shadow300((float32_2_t) {x - 1500, y - 900}, &shadow, NULL);
            *near = (float32_2_t) {shadow.x + 1500, shadow.y + 900};
        }
    }
    return false;
}

const near_map_t ds2022_map = {
    .user_data = NULL,
    .get_nearest = ds2022_map_nearest,
};
