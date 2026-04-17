CC = gcc
CFLAGS = -Wall -Wextra -O2

all: saucisse

saucisse: saucisse.c
	$(CC) $(CFLAGS) -o saucisse main.c saucisse.c tamagoshi_saucisse.c

clean:
	rm -f saucisse