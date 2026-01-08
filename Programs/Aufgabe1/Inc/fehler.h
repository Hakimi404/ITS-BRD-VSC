#ifndef FEHLER_H
#define FEHLER_H

#define ERR_OK             0
#define ERR_STACK_EMPTY   -1
#define ERR_STACK_FULL    -2
#define ERR_DIV_ZERO      -3
#define ERR_OVERFLOW      -4
#define ERR_UNKNOWN       -99

void errorHandler(int err);

#endif
