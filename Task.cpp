#include "taskBoard.hpp"
#include <fstream>
#include <iostream>

Task::Task(int id, std::string description) {
  this->id = id;
  this->description = description;
}

void Task::updateDescription(std::string description) {
  this->description = description;
}

int Task::getId(void) {
  return id;
}

std::string Task::getDescription(void) {
  return description;
}

void Task::storeTask(std::ofstream& file, int storyId, int column) {
  file << "T," << storyId << "," << getId() << "," << column << std::endl;
  file << getDescription() << std::endl;
}


