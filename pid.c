// ****************************************************
//
// Authors: Lex Harrison, Michael Zhu
// Date: 18/05/2023
// Description: The PID module of the helicopter
// which uses PID structs to calculate duty percentages
//
// ****************************************************

#include <stdint.h>

#include "pid.h"
#include "utils.h"

// Take in a PID and new error and give a new duty percentage
int32_t getDuty(struct PID *pid, int32_t error) {
    // Process P
    float pValue = pid->Kp * error;

    // Process I
    pid->summation += error;
    pid->summation = clamp(pid->summation, -3000, 3000); // Limit the cumulative error
    float iValue = pid->Ki * pid->summation;

    // Process D
    float dValue = pid->Kd * (error - pid->prevError);
    pid->prevError = error;

    // Add the components
    int32_t control = pValue + iValue + dValue + pid->offset;

    return clamp(control, 2, 98);
}

// Reset the PID state for next take off
void resetPID(struct PID *pid) {
    pid->summation = 0;
    pid->prevError = 0;
}
