#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int sigq, count = 0;

void handler(int sig) {
    printf("Caught signal: %s\n", strsignal(sig));
    if (sig == sigq && ++count == 3) exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <timeout> <signalQ> <signal1> ... <signalN>\n", argv[0]);
        exit(1);
    }

    int timeout = atoi(argv[1]);
    sigq = -1;

    for (int i = 2; i < argc; i++) {
        int matched = 0;
        for (int j = 1; j < NSIG; j++) {
            if (strcasecmp(strsignal(j), argv[i]) == 0 || strcasecmp(sys_siglist[j], argv[i]) == 0) {
                if (i == 2) sigq = j;
                if (signal(j, handler) == SIG_ERR) {
                    perror("signal");
                    exit(1);
                }
                matched = 1;
                break;
            }
        }
        if (!matched) {
            fprintf(stderr, "Unknown signal: %s\n", argv[i]);
            exit(1);
        }
    }

    if (sigq == -1) {
        fprintf(stderr, "Unknown signalQ\n");
        exit(1);
    }

    int counter = 0;
    while (1) {
        printf("%d: %d\n", getpid(), counter++);
        sleep(timeout);
    }
    return 0;
}
