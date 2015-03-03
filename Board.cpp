#include <list>
#include <fstream>
#include <iostream>
#include "taskBoard.hpp"

bool Board::loadBoard (std::string fileName) {
  
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
  int id, column, storyID;
  bool complete;
  
  // Parse the saved text file
  if (file.is_open()) {
    while (!file.eof()) {
      if(file.peek() == 'S') {
        // This is story data
        // Get the data and the description
        getline(file, line);
        storyCSV.push_back(line);
        getline(file, line);
        storyCSV.push_back(line);
      } else if (file.peek() == 'T') {
        // This is task data
        // Get the data and the description
        getline(file, line);
        taskCSV.push_back(line);
        getline(file, line);
        taskCSV.push_back(line);
      } else {
        // Unknown data, skip it
        getline(file, line);
      }
    }

    // First add all the stories
    while(storyCSV.size() >= 2) {
      data = storyCSV.front();
      storyCSV.pop_front();
      description = storyCSV.front();
      storyCSV.pop_front();
      data.erase(0, 2); // Remove identifier
      storyID = std::stoi(data.substr(0, data.find(",")));
      complete = std::stoi(data.substr(data.find(",")+1));
      this->loadStory(storyID, description, complete);
    } 

    // Then add all the tasks
    while(taskCSV.size() >= 2) {
      data = taskCSV.front();
      taskCSV.pop_front();
      description = taskCSV.front();
      taskCSV.pop_front();
      data.erase(0, 2); // Remove identifier
      storyID = std::stoi(data.substr(0, data.find(",")));
      data.erase(0,data.find(",")+1);
      id = std::stoi(data.substr(0, data.find(",")));
      column = std::stoi(data.substr(data.find(",")+1));
      this->getStory(storyID).loadTask(id, description, column);
    } 
  } else {
    return false;
  }
}
