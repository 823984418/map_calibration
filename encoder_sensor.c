//
// Created by q8239 on 2022/7/31.
//

#include "encoder_sensor.h"

#include <stdint.h>

#ifdef __MSP430F5529__
#define ENCODER_SENSOR_ENABLE
#include <msp430.h>
#endif

int32_t left_encoder;
int32_t right_encoder;

int64_t encoder_x;
int64_t encoder_y;

extern const int32_t sinz[CIRCLE];
extern const int32_t cosz[CIRCLE];

# define PI_2_FLOAT32 (2 * 3.14159265358979323846f)

void encoder_sensor_init(void) {
    left_encoder = 0;
    right_encoder = 0;
    encoder_x = 0;
    encoder_x = 0;
#ifdef ENCODER_SENSOR_ENABLE
    P2DIR &= ~(BIT0 | BIT2);
    P2IES &= ~(BIT0 | BIT2);
    P2IE |= BIT0 | BIT2;
    P8DIR &= ~(BIT2 | BIT1);
#endif
}

float encoder_get_angle(void) {
    return (float) (right_encoder - left_encoder) * PI_2_FLOAT32 / CIRCLE;
}

void encoder_get_position(float *x, float *y) {
    *x = encoder_x * POSITION_TO_MM;
    *y = encoder_y * POSITION_TO_MM;
}

#ifdef ENCODER_SENSOR_ENABLE
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void) {
    switch (P2IV) {
        case P2IV_P2IFG0: {
            if (P2IN & BIT0) {
                if (P8IN & BIT2) {
                    right_encoder++;
                    uint16_t index = ((right_encoder - left_encoder) % CIRCLE + CIRCLE) % CIRCLE;
                    encoder_x += cosz[index];
                    encoder_y += sinz[index];
                } else {
                    uint16_t index = ((right_encoder - left_encoder) % CIRCLE + CIRCLE) % CIRCLE;
                    encoder_x -= cosz[index];
                    encoder_y -= sinz[index];
                    right_encoder--;
                }
            }
            P2IFG &= ~BIT0;
            break;
        }
        case P2IV_P2IFG2: {
            if (P2IN & BIT2) {
                if (P8IN & BIT1) {
                    left_encoder--;
                    uint16_t index = ((right_encoder - left_encoder) % CIRCLE + CIRCLE) % CIRCLE;
                    encoder_x -= cosz[index];
                    encoder_y -= sinz[index];
                } else {
                    uint16_t index = ((right_encoder - left_encoder) % CIRCLE + CIRCLE) % CIRCLE;
                    encoder_x += cosz[index];
                    encoder_y += sinz[index];
                    left_encoder++;
                }
            }
            P2IFG &= ~BIT2;
            break;
        }
    }
}
#endif

