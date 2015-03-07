all: taskBoard

taskBoard: taskBoard.o Board.o Story.o Task.o
	g++ taskBoard.o Board.o Story.o Task.o -o taskBoard -std=c++11

taskBoard.o: taskBoard.cpp
	g++ -c taskBoard.cpp -std=c++11

Board.o: Board.cpp
	g++ -c Board.cpp -std=c++11

Story.o: Story.cpp
	g++ -c Story.cpp -std=c++11

Task.o: Task.cpp
	g++ -c Task.cpp -std=c++11

clean:
	rm -rf *.o taskBoard
