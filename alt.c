// ****************************************************
//
// Authors: Lex Harrison, Michael Zhu
// Date: 18/05/2023
// Description: A module of the ENCE361 project for
// altitude ADC reading and buffering
//
// ****************************************************

// STD
#include <stdint.h>
#include <stdbool.h>

// TivaWare
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"

// Custom
#include "alt.h"

// Defines
#define BUFFER_SIZE 20 // 80 / 20 = 4Hz low pass
#define SAMPLE_RATE 80

// Circular buffer
uint32_t bufferData[BUFFER_SIZE];
uint8_t bufferIndex = 0;

// Values
int32_t landCalibration = 0;
int32_t altitude = 0;

void ADCInterrupt(void) {
    ADCSequenceDataGet(ADC0_BASE, 3, &bufferData[bufferIndex++]);

    if (bufferIndex == BUFFER_SIZE)
        bufferIndex = 0;

    // Take average of buffer
    // Process the altitude data
    int32_t average = 0;
    uint32_t i;
    for (i = 0; i < BUFFER_SIZE; i++) {
        average += bufferData[i];
    }
    average /= BUFFER_SIZE;
    altitude = -mapRange(average, landCalibration, 1241 + landCalibration, 0, 100);

    ADCIntClear(ADC0_BASE, 3); // Clear the interrupt
}

void initADC(void) {
    // Initialise ADC9
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0,
    ADC_CTL_CH9 | ADC_CTL_IE | ADC_CTL_END); // Sample channel 9, set interrupt, end
    ADCSequenceEnable(ADC0_BASE, 3); // Enable sequence 3
    ADCIntRegister(ADC0_BASE, 3, ADCInterrupt); // Set the interrupt handler
    ADCIntEnable(ADC0_BASE, 3); // Enable interrupts
}

void adcTrigger(void) {
    ADCProcessorTrigger(ADC0_BASE, 3);
}

void calibrateLand(void) {
    // Wait for buffer to be filled
    uint8_t i;
    for(i = 0; i < BUFFER_SIZE; i++) {
        ADCProcessorTrigger(ADC0_BASE, 3);
        SysCtlDelay(SysCtlClockGet() / SAMPLE_RATE);
    }

    // Take average of buffer
    int32_t average = 0;
    for (i = 0; i < BUFFER_SIZE; i++) {
        average += bufferData[i];
    }
    average /= BUFFER_SIZE;

    landCalibration = average;
}
