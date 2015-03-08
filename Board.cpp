#include <list>
#include <fstream>
#include <iostream>
#include "taskBoard.hpp"
#include "exceptions.hpp"

/**
 * Lists all Story objects currently on the board, one per line
 *
 * List format is \<id\> :\<description\> : [Complete]
 */
void Board::listStories(void) {
  for (auto story = backlog.begin(); story != backlog.end(); story++) {
    std::cout << (*story)->getId() << " : " ;
    std::cout << (*story)->getDescription();
    if ((*story)->isComplete()) {
      std::cout << " : Complete";
    }
    std::cout << std::endl;
  }
}

/**
 * Creates a new Story on the board, in the To Do section.
 * @param id ID of the new story.
 * @param description A short description of the story.
 * @throw ex_storyExists Raised if the story trying to be created already exists in the board.
 */
void Board::createStory(int id, std::string description) {
  
  if (!storyExists(id)) {
    backlog.push_back(new Story(id, description));
  } else {
    throw ex_storyExists(id);
  }
}

/**
 * Sets the complete flag of a Story to true.
 * @param id ID of the story to be completed.
 * @throw ex_noStory Raised if the story trying to be completed doesn't exist.
 */
void Board::completeStory(int id) {
  Story* story = getStory(id);
  if (story) {
    story->completeStory();
  } else {
    throw ex_noStory(id);
  }
  
}

/**
 * Loads complete Board, Story and Task state from a given save file.
 * @param fileName File where state has been saved.
 * @throw ex_File Raised if the save file could not be opened.
 */
void Board::loadBoard(std::string fileName) {
  
  // Data is stored in the following format in the file:
  
  // For a story:
  // S,id,complete
  // Story.description

  // For a task:
  // T,storyID,id,column
  // Task.description

  std::ifstream file (fileName);
  std::string line, data, description;
  std::list<std::string> storyCSV;
  std::list<std::string> taskCSV;
  int id, column, storyId;
  bool complete;
 
  // Parse the saved text file
  if (file.is_open()) {    
    while (!file.eof()) {
      if (file.peek() == 'S') {        
        // This is story data
        // Get the data and the description
        getline(file, line);
        storyCSV.push_back(line);
        if (!file.eof()) {
          getline(file, line);
        } else {
          line.clear();
        }
        storyCSV.push_back(line);      
      } else if (file.peek() == 'T') {
        // This is task data
        // Get the data and the description
        getline(file, line);
        taskCSV.push_back(line);
        if (!file.eof()) {
          getline(file, line);
        } else {
          line.clear();
        }
        taskCSV.push_back(line);
      } else {
        // Unknown data, skip it       
        getline(file, line);
      }      
    }
    file.close();
    // First add all the stories
    while(storyCSV.size() >= MIN_LINES) {
      data = storyCSV.front();
      storyCSV.pop_front();
      description = storyCSV.front();
      storyCSV.pop_front();     
      data.erase(INDEX_START, ID_INDEX_END); // Remove identifier    
      storyId = std::stoi(data.substr(INDEX_START, data.find(",")));      
      complete = std::stoi(data.substr(data.find(",")+DELIM_OFFSET));        
      loadStory(storyId, description, complete);
    }     
    // Then add all the tasks   
    while(taskCSV.size() >= MIN_LINES) {
      data = taskCSV.front();
      taskCSV.pop_front();
      description = taskCSV.front();
      taskCSV.pop_front();      
      data.erase(INDEX_START, ID_INDEX_END); // Remove identifier
      storyId = std::stoi(data.substr(INDEX_START, data.find(",")));
      data.erase(INDEX_START,data.find(",")+DELIM_OFFSET);
      id = std::stoi(data.substr(INDEX_START, data.find(",")));
      column = std::stoi(data.substr(data.find(",")+DELIM_OFFSET));
      loadTask(storyId, id, description, column);
      
    }    
  } else {
    throw ex_file(fileName);
  }
}

/**
 * Deletes a Story from the board (and from memory).
 * @param storyId ID of the story to be deleted.
 * @throw ex_noStory Raised if the story to be deleted does not exist.
 */
void Board::deleteStory(int storyId) {
  Story* toDelete;  
  for (auto story = backlog.begin(); story != backlog.end(); story++) {
    if ((*story)->getId() == storyId) {
      (*story)->clearStory();
      delete *story;
      backlog.erase(story); /* Invalidated iterator, but we're exiting anyway*/
      return;
    }
  }
  throw ex_noStory(storyId);
}

/**
 * Safely exits, by saving all data and then freeing allocated memory.
 * @param filename Name of the file where state is saved.
 */
void Board::exit(std::string filename) {
  storeBoard(filename);
  auto story = backlog.begin();
  while (story != backlog.end()) {
    (*story)->clearStory();
    delete *story;
    story = backlog.erase(story); // iterator is not invalidated
  }
}

/**
 * Creates a new task in the specified story.
 * @param storyId ID of the story where the new task will be located.
 * @param id ID of the new task.
 * @param description A short description of the task.
 * @throw ex_noStory Raised if the story where the task is to be created does not exist.
 */
void Board::createTask(int storyId, int id, std::string description) {
  Story* story = getStory(storyId);
  if (story) {
    story->createTask(id, description, false);
  } else {
    throw ex_noStory(id);
  }
}

/**
 * Lists all Task objects for a given Story, one per line.
 *
 * List format is \<id\> :\<column\> : \<description\>
 * @param storyId ID of the story whose tasks will be listed.
 * @throw ex_noStory Raised if the specified story does not exist.
 */
void Board::listTasks(int storyId) {
  Story* story = getStory(storyId);
  if (story) {
    story->listTasks();
  } else {
    throw ex_noStory(storyId);
  }
}

/**
 * Deletes a Task from the board (and from memory).
 * @param storyId ID of the story where the task to be deleted is located.
 * @param id ID of the task to be deleted.
 * @throw ex_noStory Raised if the story of the task to be deleted does not exist.
 */
void Board::deleteTask(int storyId, int id) {
  Story* story = getStory(storyId);
  if (story) {
    story->deleteTask(storyId);
  } else {
    throw ex_noStory(storyId);
  }
}

/**
 * Move a Task from it's current column to another column.
 * @param storyId ID of the story containing the task to be moved.
 * @param id ID of the task to be moved.
 * @param column Column that the task will be moved to.
 * @throw ex_noStory Raised if the specified story does not exist.
 */
void Board::moveTask(int storyId, int id, int column) {
  Story* story = getStory(storyId);
  if (story) {
    story->moveTask(id, column, false);
  } else {
    throw ex_noStory(id);
  }
}

/**
 * Update the description of a Task.
 * @param storyId ID of the story containing the task to be updated.
 * @param id ID of the task to be updated.
 * @param description The new description of the task.
 * @throw ex_noStory Raised if the specified story does not exist.
 */
void Board::updateTask(int storyId, int id, std::string description) {
  Story* story = getStory(storyId);
  if (story) {
    story->updateTask(id, description);
  } else {
    throw ex_noStory(id);
  } 
}

/**
 * Create a Story with a specified complete flag. Used for restoring state.
 * @param id ID of the task to be loaded.
 * @param description Description of the task to be loaded.
 * @param complete Value of the complete flag to be set.
 */
void Board::loadStory(int id, std::string description, bool complete) { 
  createStory(id, description);
  if (complete) {
    completeStory(id);
  }
}

/**
 * Create a Task in a Story at a specified location. Used for restoring state.
 * @param storyId ID of the story where the loaded task will be.
 * @param id ID of the task to be loaded.
 * @param description Description of the task to be loaded.
 * @param column Location where the new task will be.
 * @throw ex_noStory Raised if the specified story does not exist.
 */
void Board::loadTask(int storyId,int id,std::string description,int column) {
  Story* story = getStory(storyId);   
  if (story) {
    story->loadTask(id, description, column);
  } else {
    throw ex_noStory(id);
  } 
}

/**
 * Checks whether a Story exists on the board.
 * @param id ID of the story to be checked.
 * @return True if the story exists, false if not.
 */
bool Board::storyExists(int id) {
  if (getStory(id)) {
    return true;
  }
  return false;
}

/**
 * Stores complete Board, Story and Task state to a given save file.
 * @param filename Name of the file to save state.
 */
void Board::storeBoard(std::string filename) {
  std::ofstream file(filename);
  std::vector<Story*>::iterator story = backlog.begin();
  for (story = backlog.begin(); story != backlog.end(); story++) {
    (*story)->storeStory(file);
  }
}

/**
 * Gets the story of a certain ID on the board. Only used internally by Board.
 * @param id ID of the story to be retrieved.
 * @return A pointer to the story if it exists, NULL otherwise.
 */

Story* Board::getStory(int id) {
  for (auto story = backlog.begin(); story != backlog.end(); story++) {
    if ((*story)->getId() == id) {
      return *story;
    }
  }
  return NULL;
}

