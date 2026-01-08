#include "calc.h"
#include "display.h"
#include "stack.h"
#include "fehler.h"
#include "token.h"
#include <limits.h>


int calc(T_token token) {
    int a, b, result;

    switch (token.tok) {
        case NUMBER:
            return push(token.val);

        case PLUS:
            if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
                // Prüfung auf positiven Overflow
                if (a > 0 && b > INT_MAX - a) {
                    return ERR_OVERFLOW;
                }
                // Prüfung auf negativen Underflow (Ihr Fehlerfall)
                if (a < 0 && b < INT_MIN - a) {
                    return ERR_OVERFLOW; // Oder ERR_UNDERFLOW, falls Sie das definiert haben
                }
               
                // Wenn keine Bereichsüberschreitung vorliegt, Addition durchführen
                result = b + a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case MULT:
    if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
        // Sonderfall: Multiplikation mit 0 ist immer sicher
        if (a == 0 || b == 0) {
            return push(0);
        }
 
        // Overflow-Prüfung
        if (a > 0) { // a ist positiv
            if (b > 0) { // b ist positiv
                if (a > INT_MAX / b) return ERR_OVERFLOW;
            } else { // b ist negativ
                if (b < INT_MIN / a) return ERR_OVERFLOW; // Underflow
            }
        } else { // a ist negativ
            if (b > 0) { // b ist positiv
                if (a < INT_MIN / b) return ERR_OVERFLOW; // Underflow
            } else { // b ist negativ
                if (a < INT_MAX / b) return ERR_OVERFLOW;
            }
        }
 
        result = b * a;
        return push(result);
    }
    return ERR_STACK_EMPTY;

        case DIV:
    if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
        // 1. Prüfung: Division durch Null
        if (a == 0) {
            return ERR_DIV_ZERO;
        }
        // 2. Prüfung: Spezialfall INT_MIN / -1
        if (b == INT_MIN && a == -1) {
            return ERR_OVERFLOW;
        }
       
        result = b / a;
        return push(result);
    }
    return ERR_STACK_EMPTY;

        //  CLEAR ('C') //Löscht den Stack und die Anzeige
        case CLEAR:
            clearStack();
            clearStdout();
            return ERR_OK;

         
        case PRT: //Gibt oberstes Element aus
            return printTop();     

          
        case PRT_ALL:
            return printAll();     // Gibt gesamten Stackinhalt aus

        
        case DOUBLE: //Verdoppelt oberstes Element
            return duplicateTop();

        //  
        case SWAP:  //Vertauscht die obersten zwei Werte
            return swapTop();

        case OVERFLOW: //Überlauf beim Eingeben einer Zahl
            return ERR_OVERFLOW;

        case UNEXPECTED: //Unerwartete oder ungültige Eingabe(e)
            return ERR_UNKNOWN;

        default:
            return ERR_OK;
    }
}