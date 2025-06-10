#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void handler(int sig) {
    printf("[Caught: %s]\n", strsignal(sig));
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <delay> [SIGNAL_NAMES...]\n", argv[0]);
        exit(1);
    }

    int delay = atoi(argv[1]);
    for (int i = 2; i < argc; i++) {
        int sig = -1;
        for (int j = 1; j < NSIG; j++) {
            if (strcasecmp(strsignal(j), argv[i]) == 0 || strcasecmp(sys_siglist[j], argv[i]) == 0) {
                sig = j;
                break;
            }
        }
        if (sig != -1) {
            if (signal(sig, handler) == SIG_ERR) {
                perror("signal");
                exit(1);
            }
        }
    }

    int counter = 0;
    while (1) {
        printf("%d: %d\n", getpid(), counter++);
        sleep(delay);
    }
    return 0;
}