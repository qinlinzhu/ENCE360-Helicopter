// ****************************************************
//
// Authors: Lex Harrison, Michael Zhu
// Date: 18/05/2023
// Description: The PID module of the helicopter
// which uses PID structs to calculate duty percentages
//
// ****************************************************

#ifndef PID_H_
#define PID_H_

#include <stdint.h>

// PID structure to hold the data
struct PID {
    float Kp;
    float Ki;
    float Kd;

    int32_t offset;
    int32_t summation;
    int32_t prevError;
};

// Take in a PID and new error and give a new duty percentage
int32_t getDuty(struct PID* pid, int32_t error);

// Reset the PID state for next take off
void resetPID(struct PID *pid);

#endif
