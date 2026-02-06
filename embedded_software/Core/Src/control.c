#include "control.h"

/*
 * IMU_Update
 * Body_UpdatePose
 * Gait_Update
 * for each leg:
 *     get leg phase
 *     compute foot trajectory
 *     apply body transform
 *     compute IK
 *     update joints
 * send pulses
*/

void Hexapod_Init(Hexapod_t *hex) {

}

void Hexapod_Update(Hexapod_t *hex, float dt) {

}

void Hexapod_SetVelocity(Hexapod_t *hex, float vx, float vy, float wz) {

}

void Hexapod_EmergencyStop(Hexapod_t *hex) {

}

void Gait_Init(Gait_t *gait) {

}

void Gait_Update(Gait_t *gait, float dt) {

}

float Gait_GetLegPhase(const Gait_t *gait, uint8_t leg_index) {

}

uint8_t Gait_IsLegInSwing(const Gait_t *gait, uint8_t leg_index) {

}

void FootTrajectory_Compute(Hexapod_t *hex, uint8_t leg_index, float phase, float *x, float *y, float *z) {

}

void Body_UpdatePose(Hexapod_t *hex) {

}

void Body_ApplyIMUCompensation(Hexapod_t *hex) {

}

void Body_TransformFootPosition(const Hexapod_t *hex, uint8_t leg_index, float *x, float *y, float *z) {

}

uint8_t Leg_ComputeIK(Leg_t *leg, float foot_x, float foot_y, float foot_z) {

}

void Joint_SetTarget(Joint_t *joint, float angle_deg) {

}

void Joint_Update(Joint_t *joint, float dt) {

}

uint16_t Joint_AngleToPulse(const Joint_t *joint) {

}

void Leg_Update(Leg_t *leg, float dt) {

}

void Leg_SetFootPosition(Leg_t *leg, float x, float y, float z) {

}

float Clamp(float value, float min, float max) {

}

uint8_t IsFiniteVec3(float x, float y, float z) {

}
