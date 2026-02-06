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
#include "pca9685.h"

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

void Hexapod_Init(Hexapod_t *hex);
void Hexapod_Update(Hexapod_t *hex, float dt);
void Hexapod_SetVelocity(Hexapod_t *hex, float vx, float vy, float wz);
void Hexapod_EmergencyStop(Hexapod_t *hex);
void Gait_Init(Gait_t *gait);
void Gait_Update(Gait_t *gait, float dt);
float Gait_GetLegPhase(const Gait_t *gait, uint8_t leg_index);
uint8_t Gait_IsLegInSwing(const Gait_t *gait, uint8_t leg_index);
void FootTrajectory_Compute(Hexapod_t *hex, uint8_t leg_index, float phase, float *x, float *y, float *z);
void Body_UpdatePose(Hexapod_t *hex);
void Body_ApplyIMUCompensation(Hexapod_t *hex);
void Body_TransformFootPosition(const Hexapod_t *hex, uint8_t leg_index, float *x, float *y, float *z);
uint8_t Leg_ComputeIK(Leg_t *leg, float foot_x, float foot_y, float foot_z);
void Joint_SetTarget(Joint_t *joint, float angle_deg);
void Joint_Update(Joint_t *joint, float dt);
uint16_t Joint_AngleToPulse(const Joint_t *joint);
void Leg_Update(Leg_t *leg, float dt);
void Leg_SetFootPosition(Leg_t *leg, float x, float y, float z);
float Clamp(float value, float min, float max);
uint8_t IsFiniteVec3(float x, float y, float z);

#ifdef __cplusplus
}
#endif
#endif
