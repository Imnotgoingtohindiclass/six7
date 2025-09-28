CC = gcc
CFLAGS = -Wall -Wextra -O2
OBJ = main.o six7.o

all: six7

six7: $(OBJ)
	$(CC) $(CFLAGS) -o six7 $(OBJ)

main.o: main.c six7.h
six7.o: six7.c six7.h tokenizer.h

clean:
	rm -f $(OBJ) six7