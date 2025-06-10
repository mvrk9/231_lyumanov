#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <PID> <SIGNAL_NAME>\n", argv[0]);
        exit(1);
    }

    int sig = -1;
    for (int i = 1; i < NSIG; i++) {
        if (strcasecmp(sys_siglist[i], argv[2]) == 0 || strcasecmp(argv[2], strsignal(i)) == 0) {
            sig = i;
            break;
        }
    }

    if (sig == -1) {
        fprintf(stderr, "No such signal: %s\n", argv[2]);
        exit(1);
    }

    pid_t pid = atoi(argv[1]);
    if (kill(pid, sig) == -1) {
        perror("kill");
        exit(1);
    }

    printf("Sent signal %s to process %d\n", argv[2], pid);
    return 0;
}