#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <seconds>\n", argv[0]);
        exit(1);
    }

    int delay = atoi(argv[1]);
    int counter = 0;
    while (1) {
        printf("%d: %d\n", getpid(), counter++);
        sleep(delay);
    }
    return 0;
}