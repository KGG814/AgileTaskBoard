#include "taskBoard.hpp"
#include <fstream>
#include <iostream>

/**
 * Constructor for a Task.
 * @param id ID of the task to be created.
 * @param description A short description of the task.
 */
Task::Task(int id, std::string description) {
  this->id = id;
  this->description = description;
}

/**
 * Update the description of a given Task.
 * @param description New description for this task.
 */
void Task::updateDescription(std::string description) {
  this->description = description;
}

/**
 * Getter for the ID of this Task.
 * @return ID of this task.
 */
int Task::getId(void) {
  return id;
}

/**
 * Getter for the description of this Task.
 * @return Description of this task.
 */
std::string Task::getDescription(void) {
  return description;
}

/**
 * Store a Task in file so it can be recovered later.
 * @param file Output stream of the file we are saving in.
 * @param storyId ID of the story containing the task to save.
 * @param column The current location of this task.
 */
void Task::storeTask(std::ofstream& file, int storyId, int column) {
  file << "T," << storyId << "," << getId() << "," << column << std::endl;
  file << getDescription() << std::endl;
}


