#include "display.h"
#include "scanner.h"
#include "stack.h"
#include "calc.h"
#include "fehler.h"
#include "token.h"
#include "init.h"
#include "stm32f4xx_hal.h"

/**
 * @brief Hauptprogramm: Initialisiert Hardware und startet den Taschenrechner.
 */
int main(void) {
    initITSboard();
    initDisplay();
    clearStack();

    T_token token;
    int err;

    while (1) {
        token = nextToken();
        err = calc(token);

        if (err != ERR_OK)
            errorHandler(err);

        HAL_Delay(100);
    }
}
