CFLAGS= -g -I/usr/include/SFML -L /usr/lib -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
OBJ = button.o text.o DoubleGrid.o main.o
GUI = ./gui/*.cpp ./gui/*.h
GRAPH = ./graph/*.cpp ./graph/*.h


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

clean:
	rm -r *.o go

debug:
	gdb go
