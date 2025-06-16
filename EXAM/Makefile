CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm

all: program

program: main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f program output.csv
