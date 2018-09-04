LIBS = -lncurses
CC = gcc
EXE = main
default: $(EXE)

main.o: $(EXE).c
	$(CC) -c $(EXE).c -o $(EXE).o $(LIBS)
main: main.o
	$(CC) $(EXE).o -o $(EXE) $(LIBS)
clean:
	-rm -f $(EXE).o
	-rm -f $(EXE)

