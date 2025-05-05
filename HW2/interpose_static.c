#include <stdio.h>

void fred(int x) {
    printf("INTERPOSED fred is called with %d\n", x);
}

void john(double x) {
    printf("INTERPOSED john is called with %f\n", x);
}
