#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <sys/wait.h>


double f1(double x) {
    return 5 * pow(x, 3) - 6 * pow(x, 2) - 5 * x - 9;
}

double f2(double x) {
    return sin(-8 * x) * 5 + 1;
}

double f3(double x) {
    return pow(-5, x) - 6;
}

double f4(double x) {
    return 1.0 / (1 + exp(4 * x));
}


double f0(double a, double b, double c, double d) {
    double m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    if (d < m) m = d;
    return m;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <start> <end> <step>\n", argv[0]);
        return EXIT_FAILURE;
    }

    double start = atof(argv[1]);
    double end   = atof(argv[2]);
    double step  = atof(argv[3]);

    
    int fd = open("output.csv",
                  O_CREAT | O_TRUNC | O_WRONLY,
                  0644);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        return EXIT_FAILURE;
    }
    close(fd);

    
    int p1[2], p2[2], p3[2], p4[2];
    pipe(p1); pipe(p2); pipe(p3); pipe(p4);

    
    pid_t pid1 = fork();
    if (pid1 == 0) {
        close(p1[0]);
        for (double x = start; x <= end; x += step) {
            double y = f1(x);
            write(p1[1], &y, sizeof(y));
        }
        close(p1[1]);
        exit(EXIT_SUCCESS);
    }

    
    pid_t pid2 = fork();
    if (pid2 == 0) {
        close(p2[0]);
        for (double x = start; x <= end; x += step) {
            double y = f2(x);
            write(p2[1], &y, sizeof(y));
        }
        close(p2[1]);
        exit(EXIT_SUCCESS);
    }

    
    pid_t pid3 = fork();
    if (pid3 == 0) {
        close(p3[0]);
        for (double x = start; x <= end; x += step) {
            double y = f3(x);
            write(p3[1], &y, sizeof(y));
        }
        close(p3[1]);
        exit(EXIT_SUCCESS);
    }

    
    pid_t pid4 = fork();
    if (pid4 == 0) {
        close(p4[0]);
        for (double x = start; x <= end; x += step) {
            double y = f4(x);
            write(p4[1], &y, sizeof(y));
        }
        close(p4[1]);
        exit(EXIT_SUCCESS);
    }

    
    close(p1[1]); close(p2[1]);
    close(p3[1]); close(p4[1]);


    
    printf("x,f0(minimum of f1..f4)\n");  

    double x = start;
    double y1, y2, y3, y4;
    while (x <= end + 1e-12) {
        
        read(p1[0], &y1, sizeof(double));
        read(p2[0], &y2, sizeof(double));
        read(p3[0], &y3, sizeof(double));
        read(p4[0], &y4, sizeof(double));

        double result = f0(y1, y2, y3, y4);
        printf("%f, %f\n", x, result);

        x += step;
    }


    close(p1[0]); close(p2[0]);
    close(p3[0]); close(p4[0]);


    while (wait(NULL) > 0) { /* no-op */ }

    return EXIT_SUCCESS;
}
