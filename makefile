CFLAGS=-std=gnu++11 -g -I/usr/include/SFML -L /usr/lib -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
CC=g++
OBJ = button.o text.o DoubleGrid.o main.o Grid.o Parser.o checkbox.o runner.o GUI.o
GUI = ./gui/*.cpp ./gui/*.h
GRAPH = ./graph/*.cpp ./graph/*.h
GRID = ./graph/Grid.cpp ./graph/Grid.h
PARSER=./guts/Parser.cpp ./guts/Parser.h
RUNNER=runner.cpp runner.h


all: go

go: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o go

main.o: main.cpp $(RUNNER)
	$(CC) -c main.cpp $(CFLAGS)

button.o: $(GUI) 
	$(CC) -c ./gui/button.cpp $(CFLAGS)	

checkbox.o: $(GUI)
	$(CC) -c ./gui/checkbox.cpp $(CFLAGS)

text.o: $(GUI)
	$(CC) -c ./gui/text.cpp $(CFLAGS)

DoubleGrid.o: $(GRAPH) 
	$(CC) -c ./graph/DoubleGrid.cpp $(CFLAGS)

Grid.o: $(GRID)
	$(CC) -c ./graph/Grid.cpp $(CFLAGS)

Parser.o: $(PARSER)
	$(CC) -c ./guts/Parser.cpp $(CFLAGS)

runner.o: $(RUNNER) $(GUI) $(PARSER) $(GRAPH) $(GRID)
	$(CC) -c runner.cpp $(CFLAGS)

GUI.o: gui/GUI.cpp gui/GUI.h
	$(CC) -c ./gui/GUI.cpp $(CFLAGS)

clean:
	rm -r *.o go

debug:
	gdb go
