#include <list>
#include <fstream>
#include <iostream>
#include "taskBoard.hpp"
#include "exceptions.hpp"

void Board::listStories(void) {
  for (auto story = backlog.begin(); story != backlog.end(); story++) {
    std::cout << (*story)->getId() << " : " ;
    std::cout << (*story)->getDescription() << std::endl;
  }
}

void Board::createStory(int id, std::string description) {
  
  if (!storyExists(id)) {
    backlog.push_back(new Story(id, description));
  } else {
    throw ex_storyExists(id);
  }
}

void Board::completeStory(int id) {
  Story* story = getStory(id);
  if (story) {
    story->completeStory();
  } else {
    throw ex_noStory(id);
  }
  
}

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

void Board::deleteStory(int id) {
  Story* toDelete;  
  for (auto story = backlog.begin(); story != backlog.end(); story++) {
    if ((*story)->getId() == id) {
      delete *story;
      backlog.erase(story); // Invalidated iterator, but we're exiting anyway
      return;
    }
  }
  throw ex_noStory(id);
}

void Board::exit(std::string filename) {
  storeBoard(filename);
  auto story = backlog.begin();
  while (story != backlog.end()) {
    (*story)->clearStory();
    delete *story;
    story = backlog.erase(story); // iterator is not invalidated
  }
}

void Board::createTask(int storyId, int id, std::string description) {
  Story* story = getStory(storyId);
  if (story) {
    story->createTask(id, description);
  } else {
    throw ex_noStory(id);
  }
}

void Board::listTasks(int id) {
  Story* story = getStory(id);
  if (story) {
    story->listTasks();
  } else {
    throw ex_noStory(id);
  }
}

void Board::deleteTask(int storyId, int id) {
  Story* story = getStory(storyId);
  if (story) {
    story->deleteTask(id);
  } else {
    throw ex_noStory(id);
  }
}

void Board::moveTask(int storyId, int id, int column) {
  Story* story = getStory(storyId);
  if (story) {
    story->moveTask(id, column);
  } else {
    throw ex_noStory(id);
  }
}

void Board::updateTask(int storyId, int id, std::string description) {
  Story* story = getStory(storyId);
  if (story) {
    story->updateTask(id, description);
  } else {
    throw ex_noStory(id);
  } 
}

void Board::loadStory(int id, std::string description, bool complete) { 
  createStory(id, description);
  if (complete) {
    completeStory(id);
  }
}

void Board::loadTask(int storyId,int id,std::string description,int column) {
  Story* story = getStory(storyId);   
  if (story) {
    story->loadTask(id, description, column);
  } else {
    throw ex_noStory(id);
  } 
}

bool Board::storyExists(int id) {
  if (getStory(id)) {
    return true;
  }
  return false;
}

void Board::storeBoard(std::string filename) {
  std::ofstream file(filename);
  std::vector<Story*>::iterator story = backlog.begin();
  for (story = backlog.begin(); story != backlog.end(); story++) {
    (*story)->storeStory(file);
  }
}

Story* Board::getStory(int id) {
  for (auto story = backlog.begin(); story != backlog.end(); story++) {
    if ((*story)->getId() == id) {
      return *story;
    }
  }
  return NULL;
}

