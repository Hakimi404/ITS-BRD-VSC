#include "stack.h"
#include <limits.h>

static int stack[STACK_SIZE]; //stack-speicher max 25 Zahlen
static int top = 0;  //Zeiger das oberste Element im Stack

static void intToStr(int value, char *buffer);

int push(int wert) {  //ERR_OK bei Erfolg, ERR_STACK_FULL falls der Stack bereits voll ist
    if (top >= STACK_SIZE)
        return ERR_STACK_FULL;
    stack[top++] = wert;
    return ERR_OK;
}


int pop(int *tmp) { //ERR_OK bei Erfolg, ERR_STACK_EMPTY falls der Stack leer ist
    if (top == 0)
        return ERR_STACK_EMPTY;
    *tmp = stack[--top];
    return ERR_OK;
}


void clearStack(void) {  //alle Werte werden auf 0 gesetzt(stack zurücksetzen)
    for (int i = 0; i < top; i++)
        stack[i] = 0;
    top = 0;
}


int duplicateTop(void) { //ERR_OK bei Erfolg, ERR_STACK_EMPTY oder ERR_STACK_FULL bei Fehler
    if (top == 0)
        return ERR_STACK_EMPTY;
    if (top >= STACK_SIZE)
        return ERR_STACK_FULL;
    stack[top] = stack[top - 1];
    top++;
    return ERR_OK;
}


int swapTop(void) { //ERR_OK bei Erfolg, ERR_STACK_EMPTY wenn weniger als zwei Werte sind
    if (top < 2)
        return ERR_STACK_EMPTY;
    int tmp = stack[top - 1];
    stack[top - 1] = stack[top - 2];
    stack[top - 2] = tmp;
    return ERR_OK;
}


int printTop(void) { //ERR_OK bei Erfolg, ERR_STACK_EMPTY wenn Stack leer ist
    if (top == 0)
        return ERR_STACK_EMPTY;

    clearStdout();
    char buf[32];
    intToStr(stack[top - 1], buf);
    printStdout(buf);
    printStdout("\n");
    return ERR_OK;
}


int printAll(void) {  //ERR_OK bei Erfolg, ERR_STACK_EMPTY wenn Stack leer ist
    if (top == 0)
        return ERR_STACK_EMPTY;

    clearStdout();
    printStdout("Stackinhalt: ");
    for (int i = top - 1; i >= 0; i--) {
        char buf[32];
        intToStr(stack[i], buf);
        printStdout(buf);
        printStdout(" ");
    }
    printStdout("\n");
    return ERR_OK;
}

//value Die zu konvertierende Zahl
  //buffer Puffer, in den die Zeichenkette geschrieben wird
static void intToStr(int value, char *buffer) {
    int i = 0;
    int isNeg = 0;
   
    // Sonderfall INT_MIN direkt behandeln, da -INT_MIN einen Overflow verursacht.
    // Dies ist die sicherste Methode.
    if (value == INT_MIN) {
        // strcpy ist in <string.h>, aber wir machen es manuell, um keine Libs zu brauchen
        char min_str[] = "-2147483648"; // Annahme: 32-bit int
        int k = 0;
        while (min_str[k] != '\0') {
            buffer[k] = min_str[k];
            k++;
        }
        buffer[k] = '\0';
        return;
    }
 
    if (value < 0) {
        isNeg = 1;
        value = -value; // Jetzt sicher, da INT_MIN schon behandelt wurde
    }
 
    if (value == 0) {
        buffer[i++] = '0';
    }
 
    while (value > 0) {
        buffer[i++] = (value % 10) + '0';
        value /= 10;
    }
 
    if (isNeg)
        buffer[i++] = '-';
 
    buffer[i] = '\0';
 
    // String umdrehen
    for (int j = 0; j < i / 2; j++) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = tmp;
    }
}