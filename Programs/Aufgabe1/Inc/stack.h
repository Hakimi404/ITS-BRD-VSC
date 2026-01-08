#ifndef STACK_H
#define STACK_H

#include "fehler.h"
#include "display.h"
#include <stdbool.h>

#define STACK_SIZE 25

int push(int wert);
int pop(int *tmp);
int printTop(void);
int printAll(void);
void clearStack(void);
int duplicateTop(void);
int swapTop(void);

#endif