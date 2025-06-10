#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd2 [args...]\n", argv[0]);
        exit(1);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) { perror("pipe"); exit(1); }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]); close(pipefd[1]);
        execlp(argv[1], argv[1], NULL);
        perror("execlp"); exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]); close(pipefd[0]);
        execvp(argv[2], &argv[2]);
        perror("execvp"); exit(1);
    }

    close(pipefd[0]); close(pipefd[1]);

    int status;
    pid_t wpid;
    while ((wpid = wait(&status)) > 0) {
        printf("Child %d exited with %d\n", wpid, WEXITSTATUS(status));
    }

    if (errno != ECHILD) {
        perror("wait");
        exit(1);
    }
    return 0;
}