#ifndef __CONTROL_H__
#define __CONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stm32f4xx_hal.h"
#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "mpu6050.h"

typedef struct {
    float angle;          // current servo angle (deg)
    float target_angle;   // desired servo angle (deg)
    float speed;          // speed limit (deg/s)
    float offset;         // calibration offset (deg)
    uint16_t pulse;       // final PCA9685 pulse count (0–4095)
} Joint_t;

typedef struct {
    Joint_t coxa;     // hip yaw joint
    Joint_t femur;    // hip pitch joint
    Joint_t tibia;    // knee joint
    
    float foot_x;     // foot position (mm) — IK result
    float foot_y;
    float foot_z;

    uint8_t is_in_contact; // 1 if foot touches ground
} Leg_t;

typedef struct {
    float step_height;   // lift height of foot
    float step_length;   // forward distance per step
    float phase;         // gait cycle 0–360°
    float body_roll;
    float body_pitch;
    float body_yaw;
} Gait_t;

typedef struct {
    Leg_t legs[6];   // legs[0] = front-left, etc.
    MPU6050_Data_t imu;
    Gait_t gait;

    float body_x;
    float body_y;
    float body_z;

    float vx;    // forward speed (mm/s)
    float vy;    // strafe speed
    float wz;    // yaw rotation (deg/s)
} Hexapod_t;


#ifdef __cplusplus
}
#endif
#endif
