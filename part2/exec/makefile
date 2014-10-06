CC = g++
OBJ = progA.o
BIN = progA

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

.c.o:
	$(CC) -c $<

clean:
	rm *.o $(BIN)
