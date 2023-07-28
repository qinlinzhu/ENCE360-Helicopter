// ****************************************************
//
// Authors: Lex Harrison, Michael Zhu
// Date: 18/05/2023
// Description: A module of the ENCE361 project for
// altitude ADC reading and buffering
//
// ****************************************************

#ifndef ALT_H_
#define ALT_H_

extern int32_t altitude;

void initADC(void);

void calibrateLand(void);

void adcTrigger(void);

#endif
