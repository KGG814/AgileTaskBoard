#include <iostream>
#include "taskBoard.hpp"

#define DATA_FILE "data.csv"

int main(int argc, char* argv[]) {
  Board board;
  if (board.loadBoard(DATA_FILE)) {
    std::cout << "Save data loaded" << std::endl;
  } else {
    std::cout << "Save data not found" << std::endl;
  }
}


