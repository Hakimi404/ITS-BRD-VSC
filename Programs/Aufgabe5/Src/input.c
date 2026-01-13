#include "input.h"
#include "gpio.h"
#include "timer.h"
#include <stdint.h>

/* ===== Shared encoder state ===== */
volatile uint32_t encoder_timestamp = 0;
volatile int32_t  encoder_phase_count = 0;
static uint8_t    last_phase = 0;

/* ===== Fast phase decoder ===== */
static inline int8_t decode_phase(uint8_t old, uint8_t now)
{
    static const int8_t table[16] = {
         0, -1,  1,  0,
         1,  0,  0, -1,
        -1,  0,  0,  1,
         0,  1, -1,  0
    };
    return table[(old << 2) | now];
}

/* ===== Encoder ISR logic ===== */
void encoder_isr(void)
{
    uint32_t now = getTimeStamp();   // timestamp FIRST

    uint8_t a = readGPIOPin(AUX_PORT, AUX0);
    uint8_t b = readGPIOPin(AUX_PORT, AUX1);
    uint8_t phase = (a << 1) | b;

    int8_t step = decode_phase(last_phase, phase);

    if (step != 0) {
        encoder_phase_count += step;
        encoder_timestamp = now;
        last_phase = phase;
    }
}

/* ===== Reset button S7 ===== */
bool resetpressed(void)
{
    return (readGPIOPin(BUTTON_PORT, S7) == 0);
}
