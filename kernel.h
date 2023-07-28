#ifndef KERNAL_H_
#define KERNAL_H_

// STD
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"


void initKernel(void);

void tickIntHandler(void);

void kernelStart(void);

#endif
