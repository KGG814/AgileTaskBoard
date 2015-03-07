#include <list>
#include <fstream>
#include <iostream>
#include "taskBoard.hpp"

bool Board::listStories(void) {
  for (auto story = backlog.begin(); story != backlog.end(); story++) {
    std::cout << (*story)->getId() << " : " ;
    std::cout << (*story)->getDescription() << std::endl;
  }
  return true;
}

bool Board::createStory(int id, std::string description) {
  
  if(!storyExists(id)) {
    backlog.push_back(new Story(id, description));
    return true;
  }
  error = ERROR_STORY_EXISTS;
  return false;
}

bool Board::completeStory(int id) {
  Story* story = getStory(id);
  if (story) {
    story->completeStory();
    return true;
  }
  error = ERROR_NO_STORY;
  return false;
}

bool Board::loadBoard(std::string fileName) {
  
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
      if(file.peek() == 'S') {
        
        // This is story data
        // Get the data and the description
        getline(file, line);
        storyCSV.push_back(line);
        if(!file.eof()) {
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
        if(!file.eof()) {
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
    while(storyCSV.size() >= 2) {
      data = storyCSV.front();
      storyCSV.pop_front();
      description = storyCSV.front();
      storyCSV.pop_front();     
      data.erase(0, 2); // Remove identifier    
      storyId = std::stoi(data.substr(0, data.find(",")));      
      complete = std::stoi(data.substr(data.find(",")+1));        
      loadStory(storyId, description, complete);
    } 
    
    // Then add all the tasks
    
    while(taskCSV.size() >= 2) {
      data = taskCSV.front();
      taskCSV.pop_front();
      description = taskCSV.front();
      taskCSV.pop_front();      
      data.erase(0, 2); // Remove identifier
      storyId = std::stoi(data.substr(0, data.find(",")));
      data.erase(0,data.find(",")+1);
      id = std::stoi(data.substr(0, data.find(",")));
      column = std::stoi(data.substr(data.find(",")+1));
      loadTask(storyId, id, description, column);
      
    } 
    
    return true;
    
  } else {
    error = ERROR_FILE;
    return false;
  }
}

bool Board::deleteStory(int id) {
  Story* toDelete;  
  for (auto story = backlog.begin(); story != backlog.end(); story++) {
    if ((*story)->getId() == id) {
      delete *story;
      backlog.erase(story); // Invalidated iterator, but we're exiting anyway
      return true;
    }
  }
  error = ERROR_NO_STORY;
  return false;
}

bool Board::exit(std::string filename) {
  storeBoard(filename);
  auto story = backlog.begin();
  while (story != backlog.end()) {
    (*story)->clearStory();
    delete *story;
    story = backlog.erase(story); // iterator is not invalidated
  }
  return true;
}

bool Board::createTask(int storyId, int id, std::string description) {
  Story* story = getStory(storyId);
  if (getStory(storyId)) {
    return story->createTask(id, description);
  }
  error = ERROR_NO_STORY;
  return false;

}

bool Board::listTasks(int storyId) {
  Story* story = getStory(storyId);
  if (getStory(storyId)) {
    return story->listTasks();
  }
  error = ERROR_NO_STORY;
  return false;
}

bool Board::deleteTask(int storyId, int id) {
  Story* story = getStory(storyId);
  if (getStory(storyId)) {
    return story->deleteTask(id);
  }
  error = ERROR_NO_STORY;
  return false;
}

bool Board::moveTask(int storyId, int id, int column) {
  Story* story = getStory(storyId);
  if (getStory(storyId)) {
    return story->moveTask(id, column);
  }
  error = ERROR_NO_STORY;
  return false;
}

bool Board::updateTask(int storyId, int id, std::string description) {
  Story* story = getStory(storyId);
  if (getStory(storyId)) {
    return story->updateTask(id, description);
  }
  error = ERROR_NO_STORY;
  return false;
}

bool Board::loadStory(int id, std::string description, bool complete) {
  
  if (createStory(id, description)) {
    if (complete) {
      return completeStory(id);
    }
    return true;
  }
  // error already set to  ERROR_STORY_EXISTS;
  return false; 
}

bool Board::loadTask(int storyId,int id,std::string description,int column) {
  
  Story* story = getStory(storyId);
    
  if (story) {
    return story->loadTask(id, description, column);
  }
  error = ERROR_NO_STORY;
  return false;
}

bool Board::storyExists(int id) {
  if (getStory(id)) {
    return true;
  }
  return false;
}

bool Board::storeBoard(std::string filename) {
  std::ofstream file(filename);
  std::vector<Story*>::iterator story = backlog.begin();
  for (story = backlog.begin(); story != backlog.end(); story++) {
    (*story)->storeStory(file);
  }
  return true;
}

Story* Board::getStory(int id) {
  for (auto story = backlog.begin(); story != backlog.end(); story++) {
    if ((*story)->getId() == id) {
      return *story;
    }
  }
  return NULL;
}

