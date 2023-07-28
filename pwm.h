#ifndef PWM_H_
#define PWM_H_

void initPWM(void);

void writeMainPWM(int8_t duty);

void writeTailPWM(int8_t duty);

void writeMainState(bool on);

void writeTailState(bool on);

int8_t getMainDuty();

int8_t getTailDuty();

#endif
