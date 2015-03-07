#include <iostream>
#include <map>
#include <list>
#include <boost/algorithm/string.hpp>
#include "taskBoard.hpp"

#define DATA_FILE "data.csv"

void initializeFunctionMap(void);
bool run(int argc, char* argv[], Board &board);
bool parse(std::string command, Board &board);
void printErrors(void);
int error = NO_ERROR;

enum Function { NotDefined,
                Create,
                List,
                Delete,
                Complete,
                Move,
                Update};

static std::map<std::string, Function> functionMap;
int main(int argc, char* argv[]) {
  Board board;
  if (board.loadBoard(DATA_FILE)) {
    std::cout << "Save data loaded" << std::endl;
  } else {
    std::cout << "Save data not found" << std::endl;
  }
  initializeFunctionMap();
  printErrors();
  if (run(argc, argv, board)) {
    board.exit(DATA_FILE);
    return 0;
  } else {
    board.exit(DATA_FILE);
    return 1;
  }
}

bool run(int argc, char* argv[], Board &board) {
  bool noError = true;
  for (int i = 1; i < argc; i++) { 
    if (!parse(argv[i], board)) {
      
      noError = false;
      printErrors();
    }
  }
  for (std::string command; std::getline(std::cin, command);) {
    if (command == "exit") {
      return noError;
    }
    if (!parse(command, board)) {
      
      noError = false;
      printErrors();
    }
  }
  return noError;
}

bool parse(std::string command, Board &board) {
  
  std::list<std::string> tokens;
  boost::split(tokens, command, boost::is_any_of(" "));
  if (tokens.size() < 2) {
    error = ERROR_UNDEF_CMD;
    return false;
  }
  std::string function = tokens.front();
  tokens.pop_front();
  switch (functionMap[function]) {
    
    case Create:  
      function = tokens.front();
      tokens.pop_front();
      if (function == "story") {
        if (tokens.size() >= 2) {
          int id = std::stoi(tokens.front());
          tokens.pop_front();
          return board.createStory(id, boost::algorithm::join(tokens, " "));
        } else {
          error = ERROR_NUM_ARG;
          return false;
        }    
      } else if (function == "task") {
        if (tokens.size() >= 3) {
          int storyId = std::stoi(tokens.front());
          tokens.pop_front();
          int id = std::stoi(tokens.front());
          tokens.pop_front();
          return board.createTask(storyId, id,
                                  boost::algorithm::join(tokens, " "));
        } else {
          error = ERROR_NUM_ARG;
          return false;
        }
      } else {  
        error = ERROR_UNDEF_CMD;
        return false;
      }
    case List:
      function = tokens.front();
      tokens.pop_front();
      if (function == "stories") {
        board.listStories();
        return true;
      } else if (function == "tasks") {
        if (!tokens.empty()) {
          int storyId = std::stoi(tokens.front());
          return board.listTasks(storyId);
        } else {
          error = ERROR_NUM_ARG;
          return false;
        }
      } else {  
        error = ERROR_UNDEF_CMD;
        return false;
      }
    case Delete:
      function = tokens.front();
      tokens.pop_front();
      if (function == "story") {
        if (!tokens.empty()) {
          int id = std::stoi(tokens.front());
          return board.deleteStory(id);
        } else {
          error = ERROR_NUM_ARG;
          return false;
        }  
      } else if (function == "task") {
        if (tokens.size() >= 2) {
          int storyId = std::stoi(tokens.front());
          tokens.pop_front();
          int id = std::stoi(tokens.front());
          tokens.pop_front();
          return board.deleteTask(storyId, id);
        } else {
          error = ERROR_NUM_ARG;
          return false;
        }
      } else {  
        error = ERROR_UNDEF_CMD;
        return false;
      }
    case Complete:
      function = tokens.front();
      tokens.pop_front();
      if (function == "story") {
        if (!tokens.empty()) {
          int id = std::stoi(tokens.front());
          return board.completeStory(id);
        } else {
          error = ERROR_NUM_ARG;
          return false;
        }
      } else {  
        error = ERROR_UNDEF_CMD;
        return false;
      }
    case Move:
      function = tokens.front();
      tokens.pop_front();
      if (function == "task") {
        if (tokens.size() >= 3) {
          int storyId = std::stoi(tokens.front());
          tokens.pop_front();
          int id = std::stoi(tokens.front());
          tokens.pop_front();
          int column;
          std::string columnString = boost::algorithm::join(tokens, " ");
          if (columnString == "To Do") {
            column = TODO;
          } else if (columnString == "In Process") {
            column = IN_PROCESS;
          } else if (columnString == "To Verify") {
            column = TO_VERIFY;
          } else if (columnString == "Done") {
            column = DONE;
          } else {
            error = ERROR_UNDEF_ARG;
            return false;
          }
          return board.moveTask(storyId, id, column);
        } else {
          error = ERROR_NUM_ARG;
          return false;
        }
      } else {  
        error = ERROR_UNDEF_CMD;;
        return false;
      }
    case Update:
      function = tokens.front();
      tokens.pop_front();
      if (function == "task") {
        if (tokens.size() >= 3) {
          int storyId = std::stoi(tokens.front());
          tokens.pop_front();
          int id = std::stoi(tokens.front());
          tokens.pop_front();
          return board.updateTask(storyId, id,
                                  boost::algorithm::join(tokens, " "));
        } else {
          error = ERROR_NUM_ARG;
          return false;
        }
      } else {  
        error = ERROR_UNDEF_CMD;
        return false;
      }
    default:
      error = ERROR_UNDEF_CMD;
      return false;
  }
}

void initializeFunctionMap(void) {
  functionMap["create"] = Create;
  functionMap["list"] = List;
  functionMap["delete"] = Delete;
  functionMap["complete"] = Complete;
  functionMap["move"] = Move;
  functionMap["update"] = Update;
}

void printErrors(void) {
  switch (error) {
    case NO_ERROR:
      break;
    case ERROR_NO_STORY:
      std::cout << "Specified story not found" << std::endl;
      break;
    case ERROR_NO_TASK:
      std::cout << "Specified task not found" << std::endl;
      break;
    case ERROR_COLUMN:
      std::cout << "Task is already in that column" << std::endl;
      break;
    case ERROR_INTEGRITY:
      std::cout << "Task has invalid location" << std::endl;
      break;
    case ERROR_FILE:
      std::cout << "Save data could not be opened" << std::endl;
      break;
    case ERROR_STORY_EXISTS:
      std::cout << "Story with that ID already exists" << std::endl;
      break;
    case ERROR_TASK_EXISTS:
      std::cout << "Task with that ID already exists" << std::endl;
      break;
    case ERROR_UNDEF_ARG:
      std::cout << "Undefined column argument" << std::endl;
      break;
    case ERROR_NUM_ARG:
      std::cout << "Not enough arguments" << std::endl;
      break;
    case ERROR_UNDEF_CMD:
      std::cout << "Command Not Found" << std::endl;
      break;
    default:
      std::cout << "Undefined Error" << std::endl;
      break;
  }
  error = NO_ERROR;
}


