#include <iostream>
#include <map>
#include <list>
#include <boost/algorithm/string.hpp>
#include "taskBoard.hpp"
#include "exceptions.hpp"

#define DATA_FILE "data.csv"

bool run(int argc, char* argv[], Board &board);
bool parse_catch(std::string command, Board &board);
void parse(std::string command, Board &board);
void initializeFunctionMap();

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
  try {
    board.loadBoard(DATA_FILE);
  } catch (ex_file &e){
    std::cout << "File " << e.file << " could not be opened." << std::endl; 
    return 1;
  }
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
  initializeFunctionMap();
  for (int i = FIRST_COMMAND; i < argc; i++) {
    if (std::string(argv[i]) == "exit") {
      return noError;
    } else if (!parse_catch(argv[i], board)) {
      noError = false;
    }
  }
  for (std::string command; std::getline(std::cin, command);) {
    if (command == "exit") {
      return noError;
    } else if (!parse_catch(command, board)) {
      noError = false;
    }
  }
  return noError;
}

bool parse_catch(std::string command, Board &board) {
  try {   
    parse(command, board);
  } catch (ex_noStory &e) {
    std::cout << "Story " << e.id << " does not exist." << std::endl;
    return false;
  } catch (ex_noTask &e) {
    std::cout << "Task " << e.id << " does not exist." << std::endl;
    return false;
  } catch (ex_column &e) {
    std::cout << "Task " << e.id << " already in that location." << std::endl;
    return false;
  } catch (ex_integrity &e) {    
    std::cout << e.msg << std::endl;
    return false;
  } catch (ex_storyExists &e) {
    std::cout << "Story " << e.id << " already exists." << std::endl;
    return false;
  } catch (ex_taskExists &e){
    std::cout << "Task " << e.id << " already exists." << std::endl;
    return false;
  } catch (ex_undefArg &e){
    std::cout << "Undefined arguments: \"" << e.command << "\"" << std::endl;
    return false;
  } catch (ex_numArg &e){
    std::cout << "Not enough arguments: \"" << e.command << "\"" << std::endl;
    return false;
  } catch (ex_undefCmd &e) {
    std::cout << "Undefined command: \"" << e.command << "\"" << std::endl;
    return false;
  }
}

void parse(std::string command, Board &board) {
  
  std::list<std::string> tokens;
  boost::split(tokens, command, boost::is_any_of(" "));
  if (tokens.size() < MIN_ARGS) {
    throw ex_undefCmd(command);
  }
  std::string function = tokens.front();
  tokens.pop_front();
  switch (functionMap[function]) {
    case Create:  
      function = tokens.front();
      tokens.pop_front();
      if (function == "story") {
        if (tokens.size() >= CREATE_STORY_ARGS) {
          int id = std::stoi(tokens.front());
          tokens.pop_front();
          board.createStory(id, boost::algorithm::join(tokens, " "));
          break;
        } else {
          throw ex_numArg(command);
        }    
      } else if (function == "task") {
        if (tokens.size() >= CREATE_TASK_ARGS) {
          int storyId = std::stoi(tokens.front());
          tokens.pop_front();
          int id = std::stoi(tokens.front());
          tokens.pop_front();
          board.createTask(storyId, id, boost::algorithm::join(tokens, " "));
          break;
        } else {
          throw ex_numArg(command);
        }
      } else {  
        throw ex_undefCmd(command);
      }
    case List:
      function = tokens.front();
      tokens.pop_front();
      if (function == "stories") {
        board.listStories();
        break;
      } else if (function == "tasks") {
        if (!tokens.empty()) {
          int storyId = std::stoi(tokens.front());
          board.listTasks(storyId);
          break;
        } else {
          throw ex_numArg(command);
        }
      } else {  
        throw ex_undefCmd(command);
      }
    case Delete:
      function = tokens.front();
      tokens.pop_front();
      if (function == "story") {
        if (!tokens.empty()) {
          int id = std::stoi(tokens.front());
          board.deleteStory(id);
          break;
        } else {
          throw ex_numArg(command);
        }  
      } else if (function == "task") {
        if (tokens.size() >= DELETE_TASK_ARGS) {
          int storyId = std::stoi(tokens.front());
          tokens.pop_front();
          int id = std::stoi(tokens.front());
          tokens.pop_front();
          board.deleteTask(storyId, id);
          break;
        } else {
          throw ex_numArg(command);
        }
      } else {  
        throw ex_undefCmd(command);
      }
    case Complete:
      function = tokens.front();
      tokens.pop_front();
      if (function == "story") {
        if (!tokens.empty()) {
          int id = std::stoi(tokens.front());
          board.completeStory(id);
          break;
        } else {
          throw ex_numArg(command);
        }
      } else {  
        throw ex_undefCmd(command);
      }
    case Move:
      function = tokens.front();
      tokens.pop_front();
      if (function == "task") {
        if (tokens.size() >= MOVE_TASK_ARGS) {
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
            throw ex_undefArg(command);
          }
          board.moveTask(storyId, id, column);
          break;
        } else {
          throw ex_numArg(command);
        }
      } else {  
        throw ex_undefCmd(command);
      }
    case Update:
      function = tokens.front();
      tokens.pop_front();
      if (function == "task") {
        if (tokens.size() >= UPDATE_TASK_ARGS) {
          int storyId = std::stoi(tokens.front());
          tokens.pop_front();
          int id = std::stoi(tokens.front());
          tokens.pop_front();
          board.updateTask(storyId, id, boost::algorithm::join(tokens, " "));
          break;
        } else {
          throw ex_numArg(command);
        }
      } else {  
        throw ex_undefCmd(command);
      }
    default:
      throw ex_undefCmd(command);
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
