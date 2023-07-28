#ifndef FLIGHT_H_
#define FLIGHT_H_

// PIDs
extern struct PID altControl;
extern struct PID yawControl;

extern int32_t altDuty;
extern int32_t yawDuty;

extern int32_t heightAdjust;
extern int32_t yawAdjust;

extern int32_t flightMode;

void calibrateLand(void);

void flightControl(void);

#endif
