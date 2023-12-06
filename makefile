CC = gcc
CFLAGS = -Wall -Werror -std=gnu99
APP = matrix_mult

all: $(APP)

$(APP): $(APP).c main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(APP)
