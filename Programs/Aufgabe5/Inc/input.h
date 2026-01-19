#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

// Shared encoder state updated in ISR
extern volatile uint32_t encoder_timestamp;
extern volatile int32_t  encoder_phase_count;
extern volatile uint8_t  encoder_error;

// Encoder ISR logic (called from EXTI callback)
void encoder_isr(void);

// Checks pushbutton S7 (active low)
bool resetpressed(void);

#endif
