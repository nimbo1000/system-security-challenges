EXE = arena

CC = gcc
FLAGS = -m32 -Werror -pedantic -std=gnu89 -z execstack
#FLAGS = -m32 -Werror -pedantic -std=gnu89
SRC = arena.c

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -f $(EXE)
