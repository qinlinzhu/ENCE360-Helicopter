#include <stdint.h>

// Takes a value which sits between a min and max and maps it to a new min and max
int32_t mapRange(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Clamps a value between two numbers
int32_t clamp(int32_t value, int32_t min, int32_t max) {
    if(value < min)
        value = min;
    else if(value > max)
        value = max;

    return value;
}
