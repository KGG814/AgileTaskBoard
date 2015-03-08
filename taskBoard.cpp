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
void initializeCommandMap();

/* Enum used for the map */
enum Command { NotDefined,
                Create,
                List,
                Delete,
                Complete,
                Move,
                Update};

/* A map that allows commands to be mapped to board methods */
static std::map<std::string, Command> commandMap;

int main(int argc, char* argv[]) {
  Board board;
  try {
    board.loadBoard(DATA_FILE);
  } catch (ex_file &e){
    std::cerr << "File " << e.file << " could not be opened." << std::endl;
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

/**
 * First runs the supplied command line inputs, then accepts commands from stdin
 * @param argc Argc of program passed through.
 * @param argv Argv of program passed through.
 * @param board Board object.
 * @return False if an error occurred at any point, true otherwise.
 */
bool run(int argc, char* argv[], Board &board) {
  bool noError = true;
  initializeCommandMap();
  for (int i = FIRST_COMMAND; i < argc; i++) {
    if (std::string(argv[i]) == "exit") {
    	/* Immediate exit on exit command */
      return noError;
    } else if (!parse_catch(argv[i], board)) {
      noError = false;
    }
  }
  for (std::string command; std::getline(std::cin, command);) {
    if (command == "exit") {
    	/* Immediate exit on exit command */
      return noError;
    } else if (!parse_catch(command, board)) {
      noError = false;
    }
  }
  return noError;
}

/**
 * Wrapper function for parse that catches all the necessary exceptions.
 * @param command The command to be parsed.
 * @param board
 * @return False if an exception was handled, true otherwise.
 */
bool parse_catch(std::string command, Board &board) {
  try {   
    parse(command, board);
  } catch (ex_noStory &e) {
    std::cerr << "Story " << e.id << " does not exist." << std::endl;
    return false;
  } catch (ex_noTask &e) {
    std::cerr << "Task " << e.id << " does not exist." << std::endl;
    return false;
  } catch (ex_column &e) {
    std::cerr << "Task " << e.id << " already in that location." << std::endl;
    return false;
  } catch (ex_integrity &e) {    
    std::cerr << e.msg << std::endl;
    return false;
  } catch (ex_storyExists &e) {
    std::cerr << "Story " << e.id << " already exists." << std::endl;
    return false;
  } catch (ex_taskExists &e){
    std::cerr << "Task " << e.id << " already exists." << std::endl;
    return false;
  } catch (ex_undefArg &e){
    std::cerr << "Undefined arguments: \"" << e.command << "\"" << std::endl;
    return false;
  } catch (ex_numArg &e){
    std::cerr << "Not enough arguments: \"" << e.command << "\"" << std::endl;
    return false;
  } catch (ex_undefCmd &e) {
    std::cerr << "Undefined command: \"" << e.command << "\"" << std::endl;
    return false;
  } catch (ex_undefCol &e) {
    std::cerr << e.msg << std::endl;
    return false;
  } catch (ex_taskDone &e) {
    std::cerr << "Task " << e.id << " is done. Cannot be moved."<< std::endl;
    return false;
  } catch (ex_progression &e) {
    std::cerr << e.msg << std::endl;
    return false;
  } catch (ex_incompleteTasks &e) {
    std::cerr << "Story " << e.id <<
    " can not be completed with incomplete tasks." << std::endl;
  } catch (ex_storyComplete &e) {
    std::cerr << "Cannot perform that operation on Story " << e.id <<
    ": story is complete" << std::endl;
  } catch (std::invalid_argument& e) {
  	std::cerr << "Invalid argument: " << command << std::endl;
  }
}

/**
 * Parses the input of the given command, using the map. Note that extra arguments are ignored,
 * except for commands with descriptions, which will assume they are part of the description.
 * @param command The command to be parsed.
 * @param board The Board object being worked on.
 * @throw ex_undefCmd Raised if the command is not recognised.
 * @throw	ex_numArg Raised if the command does not have enough arguments.
 * @throw undef_Arg Raised if an argument was undefined.
 * @throw invalid_argument Raised if stoi receives invalid input. (Raised by stoi).
 */
void parse(std::string command, Board &board) {
  
  std::list<std::string> tokens;
  /* Splitting the command on spaces*/
  boost::split(tokens, command, boost::is_any_of(" "));
  if (tokens.size() < MIN_ARGS) {
    throw ex_undefCmd(command);
  }
  /* Parse based on the first word of the command */
  std::string function = tokens.front();
  tokens.pop_front();
  /* Switch on the enum mapped to by the command map */
  switch (commandMap[function]) {
    case Create:  
      function = tokens.front();
      tokens.pop_front();
      if (function == "story") {
        if (tokens.size() >= CREATE_STORY_ARGS) {
          int id = std::stoi(tokens.front());
          tokens.pop_front();
          /* After getting id, the rest of the tokens are taken as the description */
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
          /* After getting storyId & i, the rest of the tokens are taken as the description */
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
          /* After getting storyId, id & column, the rest of the tokens are taken as description */
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
          /* After getting storyId & id, the rest of the tokens are taken as the description */
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

/**
 * Initiliazes the map with the required values.
 */
void initializeCommandMap(void) {
  commandMap["create"] = Create;
  commandMap["list"] = List;
  commandMap["delete"] = Delete;
  commandMap["complete"] = Complete;
  commandMap["move"] = Move;
  commandMap["update"] = Update;
}
