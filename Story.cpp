#include "taskBoard.hpp"
#include "exceptions.hpp"
#include <list>
#include <fstream>
#include <iostream>

/**
 * Constructor for story.
 * Since a story must be associated with the board, can only be called by the board.
 * @param id ID of the new story to be made. Cannot already exist.
 * @param description Short decription of the story.
 */

Story::Story(int id, std::string description) {
  this->id = id;
  this->description = description;
  for (int column = TODO; column <= DONE; column++) {
    std::vector<Task*> vector;
    tasks.push_back(vector);
  }
  complete = false;
}

/**
 * Creates a task and places it in the backlog of this story.
 * Since a task must have an associated story, this must be called when making a new task.
 *
 * @param id ID of the new task.
 * @param description Short description of the task.
 * @return True if the task was created, false if the task already existed.
 */

void Story::createTask(int id, std::string description) {
  if (!taskExists(id)) {
    tasks[TODO].push_back(new Task(id, description));
  } else {
    throw ex_taskExists(id);
  }
}

/**
 *
 * @param id
 * @param description
 * @param column
 * @return
 */

void Story::loadTask(int id, std::string description, int column) {
  createTask(id, description);
  if (column != TODO) {
    moveTask(id, column);
  }
}

void Story::moveTask(int id, int destination) {
  
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end(); task++) { 
      if ((*task)->getId() == id) {
        if (std::distance(tasks.begin(), column) != destination) {
          // Add to destination
          tasks[destination].push_back(*task);
          // Remove from original location
          column->erase(task); // Invalidated iterator, but we're exiting anyway
        } else {
          throw ex_column(id);
        }
      }
    }
  }
  throw ex_noTask(id);
}


void Story::completeStory(void) {
  complete = true;
}

bool Story::isComplete(void) {
  return complete;
}

void Story::deleteTask(int id) {
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end();) { 
      if ((*task)->getId() == id) {
        delete *task;
        column->erase(task); // Invalidated iterator, but we're exiting anyway
        return;
      }
    }
  }
  throw ex_noTask(id);
}

void Story::updateTask(int id, std::string description) {
  Task* task = getTask(id);
  if (getTask(id)) {
    task->updateDescription(description);
  } else {
    throw ex_noTask(id);
  }
}

bool Story::taskExists(int id) {
  if (getTask(id)) {
    return true;
  }
  return false;
}

void Story::clearStory(void) {
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end();) {      
      delete *task;
      task = column->erase(task);
    }
  }
}


int Story::getId(void) {
  return id;
}

std::string Story::getDescription(void) {
  return description;
}
;

Task* Story::getTask(int id) { 
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end(); task++) {
      if ((*task)->getId() == id) {
        return *task;
      }
    }
  }
  return NULL;
}

void Story::storeStory(std::ofstream &file) {
  file << "S," << getId() << "," << isComplete() << std::endl;
  file << getDescription() << std::endl;
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end(); task++) {
      (*task)->storeTask(file, getId(), std::distance(tasks.begin(), column));
    }
  }
}

void Story::listTasks(void) {
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end(); task++) {
      std::cout << (*task)->getId() << " : ";
      int col = std::distance(tasks.begin(), column);
      if (col == TODO) {
        std::cout << "To Do : ";
      } else if (col == IN_PROCESS) {
        std::cout << "In Process : ";
      } else if (col == TO_VERIFY) {
        std::cout << "To Verify : ";
      } else if (col == DONE) {
        std::cout << "Done : ";
      } else {
        throw ex_integrity();
      }
      std::cout << (*task)->getDescription() << std::endl;
    }
  }
}
