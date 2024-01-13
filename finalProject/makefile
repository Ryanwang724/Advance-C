CC = gcc
OBJ = data.o ui.o main.o

EXE = run
all: $(EXE)
.c.o: ; $(CC) -c $*.c

$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ)

clean:
	rm -rf $(EXE) *.o *.d core

winclean: # for windows use
	del $(EXE).exe *.o *.d *.dump file_system_state.txt  core
	rmdir /s/q  Dump	