#include <stdio.h>

void bill(char *msg) {
    printf("INTERPOSED bill is called with \"%s\"\n", msg);
}

void sam(double x) {
    printf("INTERPOSED sam is called with %f\n", x);
}
