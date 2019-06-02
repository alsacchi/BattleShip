LIBS = -lncurses
CC = gcc
EXE = main
default: $(EXE)

$(EXE).o: $(EXE).c
	$(CC) -c $(EXE).c -o $(EXE).o $(LIBS)
$(EXE): $(EXE).o
	$(CC) $(EXE).o -o $(EXE) $(LIBS)
clean:
	-rm -f $(EXE).o
	-rm -f $(EXE)

