#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

/* Shared ISR state */
extern volatile uint32_t encoder_timestamp;
extern volatile int32_t  encoder_phase_count;

/* ISR entry */
void encoder_isr(void);

/* Button */
bool resetpressed(void);

#endif
