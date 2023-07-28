// STD
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"

// Custom
#include "yaw.h"

// States
enum states {
    state00 = 0, state01 = 1, state10 = 2, state11 = 3
};

// Values
int32_t yaw = 0;
int32_t currentState;
bool yawCalibrated = false;

// Called when one of the rotary encoder pins changes
void yawInterupt(void) {
    int32_t nextState = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1); // next state of controller

    switch (currentState) {
    case state00:
        if (nextState == state10)
            yaw++;
        else if (nextState == state01)
            yaw--;
        break;
    case state10:
        if (nextState == state11)
            yaw++;
        else if (nextState == state00)
            yaw--;
        break;
    case state11:
        if (nextState == state01)
            yaw++;
        else if (nextState == state10)
            yaw--;
        break;
    case state01:
        if (nextState == state00)
            yaw++;
        else if (nextState == state11)
            yaw--;
        break;
    }
    currentState = nextState;

    // Switch signs to wrap the yaw around
    if (yaw > YAW_WRAP)
        yaw = yaw - YAW_WRAP*2;
    else if (yaw < -YAW_WRAP)
        yaw = YAW_WRAP*2 + yaw;

    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}

void refYawInterupt(void) {
    yaw = 0;
    yawCalibrated = true;
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);
}

void initYAW(void) {
    // Initialise
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // PB0, channel A, PB1, channel B
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOIntRegister(GPIO_PORTB_BASE, yawInterupt);
    GPIOIntTypeSet( GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);
    GPIOIntEnable( GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOC); // PC4, ref pin
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
    GPIOIntRegister(GPIO_PORTC_BASE, refYawInterupt);
    GPIOIntTypeSet( GPIO_PORTC_BASE, GPIO_INT_PIN_4, GPIO_RISING_EDGE);
//    GPIO_PIN_TYPE_WPU
    GPIOIntEnable( GPIO_PORTC_BASE, GPIO_INT_PIN_4);

    // Read current state
    if (yawCalibrated == true)
        currentState = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}
