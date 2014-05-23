CFLAGS= -std=c++11 -g -I/usr/include/SFML -L /usr/lib -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
OBJ = button.o text.o DoubleGrid.o main.o Grid.o Parser.o
GUI = ./gui/*.cpp ./gui/*.h
GRAPH = ./graph/*.cpp ./graph/*.h
GRID = ./graph/Grid.cpp ./graph/Grid.h
PARSER=./guts/Parser.cpp ./guts/Parser.h


all: go

go: $(OBJ)
	g++ $(OBJ) $(CFLAGS) -o go

main.o: main.cpp $(GUI)

button.o: $(GUI) 
	g++ -c ./gui/button.cpp $(CFLAGS)	

text.o: ./gui/text.cpp ./gui/text.h
	g++ -c ./gui/text.cpp $(CFLAGS)

DoubleGrid.o: $(GRAPH) 
	g++ -c ./graph/DoubleGrid.cpp $(CFLAGS)

Grid.o: $(GRID)
	g++ -c ./graph/Grid.cpp $(CFLAGS)

Parser.o: $(PARSER)
	g++ -c ./guts/Parser.cpp $(CFLAGS)

clean:
	rm -r *.o go

debug:
	gdb go
