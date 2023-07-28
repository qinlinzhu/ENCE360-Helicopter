#ifndef YAW_H_
#define YAW_H_

// STD
#include <stdint.h>
#include <stdbool.h>

// TivaWare
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

// Defines
#define YAW_WRAP 224

// Values
extern int32_t yaw;
extern int32_t currentState;
extern bool yawCalibrated;

// Functions
void initYAW(void);

void yawInterupt(void);

void refYawInterupt(void);

#endif
