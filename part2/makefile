CC = gcc
OBJ = sched.o
BIN = sched

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

.c.o:
	$(CC) -c $<

clean:
	rm *.o $(BIN)
