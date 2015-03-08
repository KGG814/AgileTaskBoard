#include "taskBoard.hpp"
#include "tests.hpp"
#include "exceptions.hpp"
#include <iostream>

#define DESCRIPTION1 "abcdefghijklmnopqrstuvwxyz"
#define DESCRIPTION2 "zyxwvutsrqponmlkjihgfedcba"
#define EMPTY        ""

int main (int argc, char* argv[]) {
  if (test_task_getDescription()) {
    std::cout << "Task::getDescription()    : Passed" << std::endl;
  } else {
    std::cout << "Task::getDescription()    : Failed" << std::endl;
  }
  if (test_task_updateDescription()) {
    std::cout << "Task::updateDescription() : Passed" << std::endl;
  } else {
    std::cout << "Task::updateDescription() : Failed" << std::endl;
  }
  if (test_task_getId()) {
    std::cout << "Task::getId()             : Passed" << std::endl;
  } else {
    std::cout << "Task::getId()             : Failed" << std::endl;
  }
  if (test_story_createTask()) {
    std::cout << "Story::createTask()       : Passed" << std::endl;
  } else {
    std::cout << "Story::createTask()       : Failed" << std::endl;
  }
  if (test_story_moveTask()) {
    std::cout << "Story::moveTask()         : Passed" << std::endl;
  } else {
    std::cout << "Story::moveTask()         : Failed" << std::endl;
  }
  if (test_story_completeStory()) {
    std::cout << "Story::completeStory()    : Passed" << std::endl;
  } else {
    std::cout << "Story::completeStory()    : Failed" << std::endl;
  }
  if (test_story_deleteTask()) {
    std::cout << "Story::deleteTasks()      : Passed" << std::endl;
  } else {
    std::cout << "Story::deleteTask()       : Failed" << std::endl;
  }
  if (test_story_clearStory()) {
    std::cout << "Story::clearStory()       : Passed" << std::endl;
  } else {
    std::cout << "Story::clearStory()       : Failed" << std::endl;
  }
  if (test_story_getId()) {
    std::cout << "Story::getId()            : Passed" << std::endl;
  } else {
    std::cout << "Story::getId()            : Failed" << std::endl;
  }
 if (test_story_getDescription()) {
    std::cout << "Story::getDescription()   : Passed" << std::endl;
  } else {
    std::cout << "Story::getDescription()   : Failed" << std::endl;
  }
  if (test_board_createStory()) {
    std::cout << "Board::createStory()      : Passed" << std::endl;
  } else {
    std::cout << "Board::createStory()      : Failed" << std::endl;
  }
  if (test_board_completeStory()) {
    std::cout << "Board::completeStory()    : Passed" << std::endl;
  } else {
    std::cout << "Board::completeStory()    : Failed" << std::endl;
  }
  if (test_board_deleteStory()) {
    std::cout << "Board::deleteStory()      : Passed" << std::endl;
  } else {
    std::cout << "Board::deleteStory()      : Failed" << std::endl;
  }
  if (test_board_createTask()) {
    std::cout << "Board::createTask()       : Passed" << std::endl;
  } else {
    std::cout << "Board::createTask()       : Failed" << std::endl;
  }
  if (test_board_deleteTask()) {
    std::cout << "Board::deleteTask()       : Passed" << std::endl;
  } else {
    std::cout << "Board::deleteTask()       : Failed" << std::endl;
  }
  if (test_board_moveTask()) {
    std::cout << "Board::moveTask()         : Passed" << std::endl;
  } else {
    std::cout << "Board::moveTask()         : Failed" << std::endl;
  }
  if (test_board_updateTask()) {
    std::cout << "Board::updateTask()       : Passed" << std::endl;
  } else {
    std::cout << "Board::updateTask()       : Failed" << std::endl;
  }
}

// Task tests
bool test_task_getDescription(void) {
  try {
    Task test1(0, DESCRIPTION1);
    Task test2(0, EMPTY);
    if (test1.getDescription() != DESCRIPTION1) {
      return false;
    }
    if (test2.getDescription() != EMPTY) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_task_updateDescription(void) {
  try {
    Task test1(0, DESCRIPTION1);
    Task test2(0, EMPTY);
    test1.updateDescription(DESCRIPTION2);
    if (test1.getDescription() != DESCRIPTION2) {
      return false;
    }
    test2.updateDescription(DESCRIPTION2);
    if (test2.getDescription() != DESCRIPTION2) {
      return false;
    }
    test1.updateDescription(EMPTY);
    if (test1.getDescription() != EMPTY) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_task_getId(void) {
  try {
    Task test1(0, EMPTY);
    Task test2(-1, EMPTY);
    if (test1.getId() != 0) {
      return false;
    }
    if (test2.getId() != -1) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  } 
}

// Story tests
bool test_story_createTask(void) {
  try {
    Story test(0, EMPTY);
    test.createTask(0, std::string(DESCRIPTION1), false);
    if (!test.taskExists(0)) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_story_moveTask(void) {
  try {
    Story test(0, EMPTY);
    test.createTask(0, std::string(DESCRIPTION1), false);
    test.moveTask(0, IN_PROCESS, false);
    if (!test.taskExists(0)) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_story_completeStory(void) {
  try {
    Story test(0, EMPTY);
    test.createTask(0, std::string(DESCRIPTION1), false);
    test.moveTask(0, DONE, true);
    test.completeStory();
    if (!test.isComplete()) {
      return false;
    } 
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_story_deleteTask(void) {
  try {
    Story test(0, EMPTY);
    test.createTask(0, std::string(DESCRIPTION1), false);
    if (!test.taskExists(0)) {
      return false;
    }
    test.deleteTask(0);
    if (test.taskExists(0)) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_story_clearStory(void) {
  try {
    Story test(0, EMPTY);
    test.createTask(0, std::string(DESCRIPTION1), false);
    if (!test.taskExists(0)) {
      return false;
    }
    test.clearStory();
    if (test.taskExists(0)) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_story_getId(void){
  try {
    Story test1(0, EMPTY);
    Story test2(-1, EMPTY);
    if (test1.getId() != 0) {
      return false;
    }
    if (test2.getId() != -1) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  } 
} 

bool test_story_getDescription(void){
  try {
    Story test1(0, EMPTY);
    Story test2(1, DESCRIPTION1);
    if (test1.getDescription() != EMPTY) {
      return false;
    }
    if (test2.getDescription() != DESCRIPTION1) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  } 
}

// Board tests


bool test_board_createStory(void){
  try {
    Board test;
    test.createStory(0, std::string(EMPTY));
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_board_completeStory(void){
  try {
    Board test;
    test.createStory(0, std::string(EMPTY));
    test.completeStory(0);
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_board_deleteStory(void) {
  try {
    Board test;
    test.createStory(0, std::string(DESCRIPTION1));
    test.deleteStory(0);
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_board_createTask(void){
  try {
    Board test;
    test.createStory(0, std::string(EMPTY));
    test.createTask(0, 0, std::string(EMPTY));
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_board_deleteTask(void){
  try {
    Board test;
    test.createStory(0, std::string(EMPTY));
    test.createTask(0, 0, std::string(EMPTY));
    test.deleteTask(0, 0);
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_board_moveTask(void){
  try {
    Board test;
    test.createStory(0, std::string(EMPTY));
    test.createTask(0, 0, std::string(EMPTY));
    test.moveTask(0, 0, IN_PROCESS);
    return true;
  } catch (...) {
    return false;
  } 
}

bool test_board_updateTask(void){
  try {
    Board test;
    test.createStory(0, std::string(EMPTY));
    test.createTask(0, 0, std::string(EMPTY));
    test.updateTask(0, 0, std::string(DESCRIPTION1));
    return true;
  } catch (...) {
    return false;
  } 
}

