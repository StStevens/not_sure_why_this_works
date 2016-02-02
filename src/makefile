CC = g++ -std=c++0x
CFLAGS = -c
SOURCES = main.cpp SudokuBoard.cpp BoardGenerator.cpp sudokuBoardReader.cpp backtrackingSolver.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = ~/171/bin/main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -ggdb $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -g $< -o $@

clean:
	rm *.o
