// STD
#include <stdint.h>
#include <stdbool.h>

// TivaWare
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

// Custom
#include "pwm.h"

// Values
#define PWM_RATE 200
#define PWM_DIVIDER_CODE SYSCTL_PWMDIV_2
#define PWM_DIVIDER 2

uint32_t pwmPeriod = 0;

void initPWM(void) {
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);

    // Enable the peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Configure pins
    GPIOPinConfigure(GPIO_PC5_M0PWM7);
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);

    PWMGenConfigure(PWM0_BASE, PWM_GEN_3,
    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2,
    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);

    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);

    pwmPeriod = SysCtlClockGet() / PWM_DIVIDER / PWM_RATE;
}

void writeMainPWM(int8_t duty) {
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, pwmPeriod);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, pwmPeriod * duty / 100);
}

void writeTailPWM(int8_t duty) {
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, pwmPeriod);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, pwmPeriod * duty / 100);
}

void writeMainState(bool on) {
    PWMOutputState(PWM0_BASE, PWM_OUT_7, on);
}

void writeTailState(bool on) {
    PWMOutputState(PWM1_BASE, PWM_OUT_5, on);
}

int8_t getMainDuty() {
    PWMPulseWidthGet(PWM0_BASE, PWM_OUT_7);
}

int8_t getTailDuty() {
    PWMPulseWidthGet(PWM1_BASE, PWM_OUT_5);
}
