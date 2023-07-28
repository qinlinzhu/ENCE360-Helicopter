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

// Defines
#define SAMPLE_RATE 80

#define MAX_TASKS 5

// Rates for each task
#define TICK_RATE 200
#define DEBOUNCE_RATE 100
#define CONTROL_RATE 20
#define SAMPLE_RATE 80
#define DEBUG_RATE 4
#define DISPLAY_RATE 5

volatile uint8_t period = 0;
typedef void (*FunctionPtr)(void);

typedef struct {
    char *id;
    FunctionPtr funcPtr;
    uint16_t freq;
    uint8_t priority;
    bool ready;
} Task_t;

Task_t tasks[MAX_TASKS] = {
    {"ADC", adcTrigger, (TICK_RATE / SAMPLE_RATE), 6, true},
    {"fightControl", flightControl, (TICK_RATE / CONTROL_RATE), 3, true},
    {"updateButton", updateButtons, (TICK_RATE / DEBOUNCE_RATE), 5, true},
    {"displayScreen", displayScreen, (TICK_RATE / DISPLAY_RATE), 4, true},
    {"serialDebug", debugSerial, (TICK_RATE / DEBUG_RATE), 2, true}
};

uint8_t i;
void tickIntHandler(void) {
    for (i = 0; i < MAX_TASKS; i++) {
        if (period % tasks[i].freq == 0)
            tasks[i].ready = true;
    }
    period++;
}

void kernelStart(void) {
    uint8_t j;
    for (j = 0; j < MAX_TASKS; j++) {
        if (tasks[j].ready == true) {
            tasks[j].ready = false;
            tasks[j].funcPtr();
        }
    }
}

int32_t taskCompare(const void* task_a, const void* task_b) {
    return (int32_t)((const Task_t*) task_a)->priority - (int32_t)((const Task_t*) task_b)->priority;
}

void kernelPrioritise(void) {
    qsort(tasks, MAX_TASKS, sizeof(Task_t), taskCompare);
}

void initKernel(void) {
    // Set up an interrupt
    SysTickPeriodSet(SysCtlClockGet() / TICK_RATE);
    SysTickIntRegister(tickIntHandler);
    SysTickIntEnable();
    SysTickEnable();

    kernelPrioritise();
}
