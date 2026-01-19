#include "input.h"
#include "gpio.h"
#include "timer.h"

// Shared encoder state
volatile uint32_t encoder_timestamp = 0;
volatile int32_t  encoder_phase_count = 0;
volatile uint8_t  encoder_error = 0;

static uint8_t last_phase = 0;

// Quadrature decode table:
// index = (old<<2)|now, value = +1/-1 for valid step, 0 for no/invalid
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

// Encoder ISR logic (called from HAL_GPIO_EXTI_Callback)
void encoder_isr(void)
{
    uint32_t now = getTimeStamp();

    // Read PF0/PF1 (S0/S1) -> phase bits
    uint8_t a = (uint8_t)readGPIOPin(BUTTON_PORT, S0);
    uint8_t b = (uint8_t)readGPIOPin(BUTTON_PORT, S1);
    uint8_t phase = (uint8_t)((a << 1) | b);

    int8_t step = decode_phase(last_phase, phase);

    if (step == 0) {
        // If phase changed but decode says "0" => invalid transition / bounce
        if (phase != last_phase) {
            encoder_error = 1;
        }
        return;
    }

    encoder_phase_count += step;
    encoder_timestamp = now;
    last_phase = phase;
}

// Checks pushbutton S7 (active low)
bool resetpressed(void)
{
    return (readGPIOPin(BUTTON_PORT, S7) == 0);
}
