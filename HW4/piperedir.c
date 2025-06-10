#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd2 [args...]\n", argv[0]);
        exit(1);
    }

    int pipefd[2];
    pipe(pipefd);
    pid_t p1 = fork();
    if (p1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]); close(pipefd[1]);
        execlp(argv[1], argv[1], NULL);
        perror("execlp cmd1"); exit(1);
    }

    pid_t p2 = fork();
    if (p2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]); close(pipefd[0]);
        execvp(argv[2], &argv[2]);
        perror("execvp cmd2"); exit(1);
    }

    close(pipefd[0]); close(pipefd[1]);
    wait(NULL); wait(NULL);
    return 0;
}
