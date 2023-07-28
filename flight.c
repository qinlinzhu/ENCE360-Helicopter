// ****************************************************
//
// Authors: Lex Harrison, Michael Zhu
// Date: 18/05/2023
// Description: A module of the ENCE361 project for
// display initialization and updates
//
// ****************************************************

// STD
#include <buttons.h>
#include <stdint.h>
#include <stdbool.h>

// TivaWare
#include "driverlib/sysctl.h"

// Custom
#include "flight.h"
#include "alt.h"
#include "pid.h"
#include "yaw.h"
#include "pwm.h"
#include "utils.h"

// Altitude control PID values
#define ALT_PID_P 3.3
#define ALT_PID_I 0.03
#define ALT_PID_D 0.01

// Yaw control PID values
#define YAW_PID_P 2.7
#define YAW_PID_I 0.02
#define YAW_PID_D 0.01

// Add a default offset for the duty percentages to assist the PID
#define ALT_DUTY_OFFSET 25
#define YAW_DUTY_OFFSET 25

// Adjustment values
#define ALT_ADJUST 10
#define YAW_ADJUST 19

// Landing threshold values
#define LANDING_YAW_THRESHOLD 5
#define LADNING_ALT_THRESHOLD 3

// Yaw calibration finding tail duty
#define YAW_CALIBRATION_DUTY 70

// PIDs
struct PID altControl = { ALT_PID_P, ALT_PID_I, ALT_PID_D, ALT_DUTY_OFFSET };
struct PID yawControl = { YAW_PID_P, YAW_PID_I, YAW_PID_D, YAW_DUTY_OFFSET };

// Flight modes
enum FlightModes {
    landed = 0, launching = 1, flying = 2, landing = 3
};

int32_t flightMode = landed;

// Current targets
int32_t heightAdjust = 10;
int32_t yawAdjust = 0;

// Error and duties
int32_t altError = 0;
int32_t yawError = 0;
int32_t altDuty = 0;
int32_t yawDuty = 0;

int32_t calculateYawError(int32_t target, int32_t current) {
    int32_t error = target - current;

    // Check if it is closer to going through the seam
    if (error > YAW_WRAP)
        error = error - YAW_WRAP*2;
    else if (error < -YAW_WRAP)
        error = YAW_WRAP*2 + error;

    return error;
}

void flightControl(void) {
    // Handle UI
    if(!checkButtonState(RESET)) {
        SysCtlReset();
    }

    if (checkButton(UP) == PUSHED)
        heightAdjust += ALT_ADJUST;
    else if (checkButton(DOWN) == PUSHED)
        heightAdjust -= ALT_ADJUST;
    else if (checkButton(LEFT) == PUSHED)
        yawAdjust += YAW_ADJUST;
    else if (checkButton(RIGHT) == PUSHED)
        yawAdjust -= YAW_ADJUST;

    // Clamp/wrap user targets
    heightAdjust = clamp(heightAdjust, 0, 100);
    if (yawAdjust > YAW_WRAP) {
        yawAdjust = yawAdjust - YAW_WRAP*2;
    } else if (yawAdjust < -YAW_WRAP) {
        yawAdjust = YAW_WRAP*2 + yawAdjust;
    }

    // Run the respective function depending on the current flight mode
    switch (flightMode) {
    case landed:
        landedMode();
        break;
    case launching:
        launchingMode();
        break;
    case flying:
        flyingMode();
        break;
    case landing:
        landingMode();
        break;
    }
}

void landedMode() {
    // Reset targets
    heightAdjust = 10;
    yawAdjust = 0;

    // Reset PIDs
    resetPID(&altControl);
    resetPID(&yawControl);

    // Turn off the motors
    writeMainPWM(0);
    writeTailPWM(0);
    writeMainState(false);
    writeTailState(false);

    // Move into the lauching state if SW is CHANGED to the up position
    if (checkButton(SWITCH) == PUSHED)
        flightMode = launching;
}

void launchingMode() {
    // Ascend to target height
    altError = heightAdjust - altitude;

    // Write duties
    yawDuty = YAW_CALIBRATION_DUTY; // Spin in a circle
    writeTailPWM(yawDuty);
    altDuty = getDuty(&altControl, altError);
    writeMainPWM(altDuty);

    // Once yaw has calibrated enter the flying mode
    if (yawCalibrated)
        flightMode = flying;
}

void flyingMode() {
    // Begin landing is the button is currently down - does not need to be a state change
    if (checkButtonState(SWITCH))
        flightMode = landing;

    // Calculate errors
    altError = heightAdjust - altitude;
    yawError = calculateYawError(yawAdjust, yaw);

    // Run PIDs using the errors
    altDuty = getDuty(&altControl, altError);
    yawDuty = clamp(getDuty(&yawControl, yawError), 2, 60);

    // Turn on the motors and write the PWM values
    writeMainState(true);
    writeTailState(true);
    writeMainPWM(altDuty);
    writeTailPWM(yawDuty);
}

uint8_t p = 0;
void landingMode() {
    // Target reference
    yawAdjust = 0;
    yawError = calculateYawError(yawAdjust, yaw);
    altError = heightAdjust - altitude;

    // Decrease height if in range or still above 10%
    if ((yaw < LANDING_YAW_THRESHOLD && yaw > -LANDING_YAW_THRESHOLD)) {
        if(p % 4 == 0)
            heightAdjust -= 1;
    }

    // Write duties
    altDuty = getDuty(&altControl, altError);
    writeMainPWM(altDuty);
    yawDuty = clamp(getDuty(&yawControl, yawError), 2, 60);
    writeTailPWM(yawDuty);

    // If everything is in range
    if (yaw < LANDING_YAW_THRESHOLD && yaw > -LANDING_YAW_THRESHOLD && altitude < LANDING_ALT_THRESHOLD)
        flightMode = landed;
    else if (altitude == 0) // If we've already touched down give up
        flightMode = landed;
}
