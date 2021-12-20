CC = gcc
SOURCES = $(wildcard *.c)
TARGET = $(SOURCES:%.c=%)
CFLAGS = -g -Wall
all: $(TARGET)

%.o: %.c
	$(CC) -c $< -o $@

pokemon-fight: pokemon-fight.o pokedex/pokemon.o pokedex/pokedex.o helpers/helpers.o
	$(CC) $^ -o $@

pokemon: pokemon.o helpers/helpers.o
	$(CC) $^ -o $@

game: game.o pokedex/pokemon.o pokedex/pokedex.o helpers/helpers.o
	$(CC) $^ -o $@

run_%: %
	./$< 

clean:
	rm -r $(TARGET)
	find ./ -name "*.o" -type f -delete