#include "taskBoard.hpp"
#include "exceptions.hpp"
#include <list>
#include <fstream>
#include <iostream>

/**
 * Constructor for Story.
 * @param id ID of the story to be created.
 * @param description A short description of the story.
 */
Story::Story(int id, std::string description) {
  this->id = id;
  this->description = description;
  /* Initialize the task list, which is organised into 4 lists: TODO, IN_PROCESS, TO_VERIFY, DONE */
  for (int column = TODO; column <= DONE; column++) {
    std::vector<Task*> vector;
    tasks.push_back(vector);
  }
  complete = false;
}

/**
 * Creates a Task and places it in the backlog of this Story.
 * Since a task must have an associated story, this must be called when making a new task.
 *
 * @param id ID of the new task.
 * @param description Short description of the task.
 * @param load Allows the completeness of the story to be ignored while loading.
 * @throw ex_storyComplete Raised if the story where we are trying to create the task is
 * marked complete, unless we are loading.
 */

void Story::createTask(int id, std::string description, bool load) {
  if (this->isComplete() && !load) {
    throw ex_storyComplete(this->getId());
  } else if (!taskExists(id)) {
    tasks[TODO].push_back(new Task(id, description));
  } else {
    throw ex_taskExists(id);
  }
}

/**
 * Creates a Task at the specified location in this Story.
 * @param id ID of the task to be created
 * @param description A short description of the task.
 * @param column Where the new task will be located.
 */

void Story::loadTask(int id, std::string description, int column) {
  createTask(id, description, true);
  if (column != TODO) {
    moveTask(id, column, true);
  }
}

/**
 * Move a Task to a specified location.
 * @param id ID of the task to be moved.
 * @param destination Where specified task will be moved to.
 * @param load Allows the completeness of the story to be ignored while loading.
 * @throw ex_storyComplete Raised if the story containing the task to be moved is complete, unless
 * we are loading
 * @throw ex_undefCol Raised if the destination is not defined.
 * @throw ex_taskDone Raised if the task to be moved is in the Done column.
 * @throw ex_progression Raised if the caller is trying to skip columns.
 * @throw ex_column Raised if the task is already at the destination.
 * @throw ex_noTask Raised if the specified task does not exist.
 */
void Story::moveTask(int id, int destination, bool load) {
  if (this->isComplete() && !load) {
    throw ex_storyComplete(this->getId());
    return;
  }
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end(); task++) { 
      if ((*task)->getId() == id) {
        int col = std::distance(tasks.begin(), column);
        if (col != destination) {
          if (destination > DONE) {
            throw ex_undefCol();
          } else if (col == DONE) {
            throw ex_taskDone(id);
          } else if (destination > col+MAX_PROGRESSION && !load){
            throw ex_progression();
          }
          // Add to destination
          tasks[destination].push_back(*task);
          // Remove from original location
          column->erase(task); // Invalidated iterator, but we're exiting anyway
          return;
        } else {
          throw ex_column(id);
          return;
        }
      }
    }
  }
  throw ex_noTask(id);
}

/**
 * Mark a specified Story as complete.
 * @throw ex_incompleteTasks Raised if the story to be completed has incomplete tasks.
 */
void Story::completeStory(void) {
  if (tasks[TODO].size() == 0 && tasks[IN_PROCESS].size() == 0 &&
      tasks[TO_VERIFY].size() == 0) {
    complete = true;
  } else {
    throw ex_incompleteTasks(this->getId());
  }
  
}

/**
 * Getter for the complete flag of this Story.
 * @return Current value of complete (true / false).
 */
bool Story::isComplete(void) {
  return complete;
}

/**
 * Delete a Task from this Story (and from memory).
 * @param id ID of the task to be deleted.
 * @throw ex_storyComplete Raised if the story where the task we are deleting is complete.
 */
void Story::deleteTask(int id) {
  if (this->isComplete()) {
    throw ex_storyComplete(this->getId());
    return;
  }
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

/**
 * Updates the description of a Task in this Story.
 * @param id ID of the task to be updated.
 * @param description The new description for the task.
 * @throw ex_noTask Raised if the task to be updated does not exist.
 */
void Story::updateTask(int id, std::string description) {
  Task* task = getTask(id);
  if (this->isComplete()) {
    throw ex_storyComplete(this->getId());
  } else if (getTask(id)) {
    task->updateDescription(description);
  } else {
    throw ex_noTask(id);
  }
}

/**
 * Check if a Task exists in the Story.
 * @param id ID of the task to be looked for.
 * @return True if the task exists, false otherwise.
 */
bool Story::taskExists(int id) {
  if (getTask(id)) {
    return true;
  }
  return false;
}

/**
 * Clears all Tasks from the Story.
 */
void Story::clearStory(void) {
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end();) {      
      delete *task;
      task = column->erase(task);
    }
  }
}

/**
 * Getter for the ID of this Story.
 * @return ID of this story.
 */
int Story::getId(void) {
  return id;
}

/**
 * Getter for the description of this Story.
 * @return Description of this story.
 */
std::string Story::getDescription(void) {
  return description;
}

/**
 * Getter for a Task in this Story. Only used internally.
 * @param id ID of the task to get.
 * @return Pointer to the Task.
 */
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

/**
 * Store this Story's state to be recovered later.
 * @param file Output stream to be written to.
 */
void Story::storeStory(std::ofstream &file) {
  file << "S," << getId() << "," << isComplete() << std::endl;
  file << getDescription() << std::endl;
  for (auto column = tasks.begin(); column != tasks.end(); column++) {
    for (auto task = column->begin(); task != column->end(); task++) {
      (*task)->storeTask(file, getId(), std::distance(tasks.begin(), column));
    }
  }
}

/**
 * Lists all Task objects for this Story, one per line.
 *
 * List format is \<id\> :\<column\> : \<description\>
 * @throw ex_integrity Raised if a task in the list has an invalid column.
 */
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
