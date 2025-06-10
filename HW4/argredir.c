#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s infile outfile command [args...]\n", argv[0]);
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if (pid == 0) {
        int in = open(argv[1], O_RDONLY);
        if (in < 0) { perror("open infile"); exit(1); }
        int out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out < 0) { perror("open outfile"); exit(1); }
        dup2(in, STDIN_FILENO);
        dup2(out, STDOUT_FILENO);
        close(in); close(out);
        execvp(argv[3], &argv[3]);
        perror("execvp");
        exit(1);
    } else {
        wait(NULL);
    }
    return 0;
}