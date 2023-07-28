// ****************************************************
//
// Authors: Lex Harrison, Michael Zhu
// Date: 18/05/2023
// Description: A module of the ENCE361 project for
// display initialization and updates
//
// ****************************************************

// STD
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
#include "display.h"
#include "yaw.h"
#include "alt.h"
#include "flight.h"

// Variables
bool oledInit = false;

// Writes a line to the OLED display with an ending string
void writeLineEnd(char *label, uint32_t value, char *end, uint8_t line) {
    OLEDStringDraw("                ", 0, line); // Clear

    char lineBuffer[17];
    usnprintf(lineBuffer, sizeof(lineBuffer), "%s%3d%s", label, value, end);

    OLEDStringDraw(lineBuffer, 0, line);
}

// Writes a line to the OLED display
void writeLine(char *label, uint32_t value, uint8_t line) {
    writeLineEnd(label, value, "", line);
}

// Display update function which writes flight details to the display
void displayScreen(void) {
    if (!oledInit)
        return;

    //convert to degrees
    int32_t degrees = (yaw * 360) / 448;

    // Altitude information
    writeLineEnd("Alt ", altitude, "%", 0);
    writeLineEnd("Yaw ", degrees, " deg", 1);
    writeLineEnd("A PWM", altDuty, "%", 2);
    writeLineEnd("Y PWM ", yawDuty, "%", 3);
}
