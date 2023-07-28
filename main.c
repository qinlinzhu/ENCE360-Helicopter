// ****************************************************
//
// Authors: Lex Harrison, Michael Zhu
// Date: 18/05/2023
// Description: The main C program which initializes
// each module of the helicopter project
//
// ****************************************************

// STD
#include <buttons.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"

// TivaWare
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"

// Others
#include "OrbitOLED/OrbitOLEDInterface.h"

// Custom
#include "pid.h"
#include "yaw.h"
#include "pwm.h"
#include "utils.h"
#include "flight.h"
#include "alt.h"
#include "kernel.h"
#include "display.h"
#include "uart.h"

// Clock initialization
void initClock(void) {
    // Set up the clock to be 20MHz
    SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_10);
}

// Main function
int main(void) {
    // Initialise modules
    initClock();
    initADC();
    initYAW();
    initButtons();
    initPWM();
    initUART();
    initKernel();

    // Calibrate ADC landed value
    calibrateLand();

    // OLED
    OLEDInitialise();
    oledInit = true;

    // Loop the kernel
    while (1) {
        kernelStart();
    }

    return 0;
}
