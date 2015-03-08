all: taskBoard

taskBoard: taskBoard.o Board.o Story.o Task.o
	g++ taskBoard.o Board.o Story.o Task.o -o taskBoard -std=c++11 -Wall -Werror

tests: tests.o Board.o Story.o Task.o
	g++ tests.o Board.o Story.o Task.o -o taskBoardTests -std=c++11 -Wall -Werror

tests.o: tests.cpp
	g++ -c tests.cpp -std=c++11 -Wall -Werror

taskBoard.o: taskBoard.cpp
	g++ -c taskBoard.cpp -std=c++11 -Wall -Werror

Board.o: Board.cpp
	g++ -c Board.cpp -std=c++11 -Wall -Werror

Story.o: Story.cpp
	g++ -c Story.cpp -std=c++11 -Wall -Werror

Task.o: Task.cpp
	g++ -c Task.cpp -std=c++11 -Wall -Werror

clean:
	rm -rf *.o taskBoard taskBoardTests
