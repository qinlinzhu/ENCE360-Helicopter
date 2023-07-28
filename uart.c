// ****************************************************
//
// Authors: Lex Harrison, Michael Zhu
// Date: 18/05/2023
// Description: A module of the ENCE361 project for
// UART functions
//
// ****************************************************

// STD
#include <buttons.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "ustdlib.h"

// TivaWare
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"

// Custom
#include "uart.h"
#include "alt.h"
#include "yaw.h"
#include "pid.h"
#include "flight.h"

// Select which UART to be used
#define UART_BASE UART0_BASE

// Configuration of the UART
#define UART_CONFIG UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE
#define BAUD_RATE 9600

// Configuration for UART pins
#define UART_PORT_PERIHPERAL SYSCTL_PERIPH_GPIOA
#define UART_PORT GPIO_PORTA_BASE
#define UART_PINS GPIO_PIN_0 | GPIO_PIN_1

// Initialize the UART port
void initUART(void) {
    // Enable the UART and GPIO peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(UART_PORT_PERIHPERAL);

    // Configure UART
    GPIOPinTypeUART(UART_PORT, UART_PINS);
    UARTConfigSetExpClk(UART_BASE, SysCtlClockGet(), BAUD_RATE, UART_CONFIG);
    UARTFIFOEnable(UART_BASE);
    UARTEnable(UART_BASE);
}

// Sends a character buffer over the UART channel
void UARTSend(char *buffer) {
    while (*buffer) {
        UARTCharPut(UART_BASE, *buffer);
        buffer++;
    }
}

// Sends/receives serial data
void debugSerial(void) {
    // Read in PID values from a custom program
    int32_t readIn = 0;
    while ((readIn = UARTCharGetNonBlocking(UART_BASE)) != -1) {
        if (readIn == 'A') {
            altControl.Kp = UARTCharGetNonBlocking(UART_BASE) / 255.0 * 4.0;
            altControl.Ki = UARTCharGetNonBlocking(UART_BASE) / 255.0 * 0.1;
            altControl.Kd = UARTCharGetNonBlocking(UART_BASE) / 255.0;

            yawControl.Kp = UARTCharGetNonBlocking(UART_BASE) / 255.0 * 4.0;
            yawControl.Ki = UARTCharGetNonBlocking(UART_BASE) / 255.0 * 0.1;
            yawControl.Kd = UARTCharGetNonBlocking(UART_BASE) / 255.0;
        }
    }

    char lineBuffer[30];
    usnprintf(lineBuffer, sizeof(lineBuffer), "A %d T %d\r\n", altitude, heightAdjust);
    UARTSend(lineBuffer);

    usnprintf(lineBuffer, sizeof(lineBuffer), "Y %d T %d\r\n", yaw, yawAdjust);
    UARTSend(lineBuffer);

    usnprintf(lineBuffer, sizeof(lineBuffer), "M %d\r\n", flightMode);
    UARTSend(lineBuffer);

    usnprintf(lineBuffer, sizeof(lineBuffer), "S %d\r\n", checkButton(SWITCH));
    UARTSend(lineBuffer);
}
