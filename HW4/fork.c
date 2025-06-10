#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid1 = fork();
    if (pid1 < 0) { perror("fork"); exit(1); }
    if (pid1 == 0) {
        printf("Hello from Child (PID %d, PPID %d)\n", getpid(), getppid());
        pid_t pid2 = fork();
        if (pid2 < 0) { perror("fork"); exit(1); }
        if (pid2 == 0) {
            printf("Hello from Grandchild (PID %d, PPID %d)\n", getpid(), getppid());
        } else {
            wait(NULL);
        }
    } else {
        wait(NULL);
        printf("Hello from Parent (PID %d)\n", getpid());
    }
    return 0;
}