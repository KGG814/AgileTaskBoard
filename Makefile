all: taskBoard

taskBoard: taskBoard.o Board.o
	g++ taskBoard.o Board.o -o taskBoard -std=c++11

taskBoard.o: taskBoard.cpp
	g++ -c taskBoard.cpp -std=c++11

Board.o: Board.cpp
	g++ -c Board.cpp -std=c++11mae

clean:
	rm -rf *.o taskBoard
