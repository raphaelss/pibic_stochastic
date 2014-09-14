FLAGS = -std=c99 -Wall -pedantic -O3
LIBS = -lm
CC = gcc
EXE = stoch_c

$(EXE): stochastic.c
	$(CC) $(FLAGS) stochastic.c $(LIBS) -o $(EXE)

clean:
	rm $(EXE)
