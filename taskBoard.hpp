#include <vector>
#include <string>
#include <exception>

#define TODO                0
#define IN_PROCESS          1
#define TO_VERIFY           2
#define DONE                3

#define FIRST_COMMAND       1

#define MIN_ARGS            2

#define CREATE_STORY_ARGS   2
#define CREATE_TASK_ARGS    3
#define DELETE_TASK_ARGS    2
#define MOVE_TASK_ARGS      3
#define UPDATE_TASK_ARGS    3

#define MIN_LINES           2


#define INDEX_START         0
#define ID_INDEX_END        2

#define DELIM_OFFSET        1


/**
 * \brief Represents a task in a story on the board.
 */

class Task {
  public:
    Task(int id, std::string description);
    void updateDescription(std::string description);
    int getId(void);
    void storeTask(std::ofstream &file, int storyId, int column);
    std::string getDescription(void);
  private:
    int id;
    std::string description;
};

/**
 * \brief Represents a story on the board.
 */
class Story {
  public:     
    Story(int id, std::string description);
    void createTask(int id, std::string description);
    void loadTask(int id, std::string description, int column);
    void moveTask(int id, int destination);                                
    void completeStory(void);
    bool isComplete(void);
    void deleteTask(int id);
    void updateTask(int id, std::string description);
    bool taskExists(int id);
    void clearStory(void);
    int getId(void);
    std::string getDescription(void);
    void storeStory(std::ofstream &file);
    void listTasks();

  private:
    int id;
    bool complete;  
    std::string description;
    std::vector<std::vector<Task*>> tasks;
    Task* getTask(int id);
};

/**
 *  \brief Representation of a Scrum task board
 */

class Board {
  public:
    void listStories(void);
    void createStory(int id, std::string description);
    void completeStory(int id);
    void loadBoard(std::string fileName);
    void deleteStory(int id);
    void exit(std::string filename);
    void createTask(int storyId, int id, std::string description);
    void listTasks(int id);
    void deleteTask(int storyId, int id);
    void moveTask(int storyId, int id, int column);
    void updateTask(int storyId, int id, std::string description);

    
  private:
    std::vector<Story*> backlog;
    void loadStory(int id, std::string description, bool complete);
    void loadTask(int storyId, int id, std::string description, int column);
    bool storyExists(int id);
    void storeBoard(std::string filename);
    Story* getStory(int id);

};


