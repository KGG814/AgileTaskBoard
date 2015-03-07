#include "taskBoard.hpp"
#include <list>
#include <fstream>
#include <iostream>

Story::Story(int id, std::string description) {
  this->id = id;
  this->description = description;
  for (int column = TODO; column <= DONE; column++) {
    std::vector<Task*> vector;
    tasks.push_back(vector);
  }
  complete = false;
}

bool Story::createTask(int id, std::string description) {
  if (!taskExists(id)) {
    tasks[TODO].push_back(new Task(id, description));
    return true;
  }
  error = ERROR_TASK_EXISTS;
  return false;
}

bool Story::loadTask(int id, std::string description, int column) {
  if (createTask(id, description)) {
    if (column != TODO) {
      return moveTask(id, column);
    }
    return true;
  }
  // error already set to ERROR_TASK_EXISTS;
  return false;
}

bool Story::moveTask(int id, int destination) {
  
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end(); task++) { 
      if ((*task)->getId() == id) {
        if (std::distance(tasks.begin(), column) != destination) {
          // Add to destination
          tasks[destination].push_back(*task);
          // Remove from original location
          column->erase(task); // Invalidated iterator, but we're exiting anyway
          return true;
        } else {
          
          error = ERROR_COLUMN;
          return false;
        }
      }
    }
  }
  error = ERROR_NO_TASK;
  return false;
}


bool Story::completeStory(void) {
  complete = true;
  return true;
}

bool Story::isComplete(void) {
  return complete;
}

bool Story::deleteTask(int id) {
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end();) { 
      if ((*task)->getId() == id) {
        delete *task;
        column->erase(task); // Invalidated iterator, but we're exiting anyway
        return true;
      }
    }
  }
  error = ERROR_NO_TASK;
  return false;
}

bool Story::updateTask(int id, std::string description) {
  Task* task = getTask(id);
  if (getTask(id)) {
    task->updateDescription(description);
    return true;
  }
  error = ERROR_NO_TASK;
  return false;
}

bool Story::taskExists(int id) {
  if (getTask(id)) {
    return true;
  }
  return false;
}

bool Story::clearStory(void) {
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end();) {      
      delete *task;
      task = column->erase(task);
    }
  }
  return true;
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

bool Story::storeStory(std::ofstream &file) {
  file << "S," << getId() << "," << isComplete() << std::endl;
  file << getDescription() << std::endl;
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end(); task++) {
      (*task)->storeTask(file, getId(), std::distance(tasks.begin(), column));
    }
  }
  return NULL;
}

bool Story::listTasks(void) {
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
        error = ERROR_INTEGRITY;
        return false;
      }
      std::cout << (*task)->getDescription() << std::endl;
    }
  }
  return true;
}
