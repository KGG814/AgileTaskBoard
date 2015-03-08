#include <vector>
#include <string>

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

#define MAX_PROGRESSION     1


/**
 * \brief Represents a task in a Story on the Board.
 *
 * Each task represents something that needs to be done to implement it's story. <br>
 * It contains a brief description of the task. <br>
 * The task can not exist without a story.
 *
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
 *
 * Each story represents a unit of work in the Agile/Scrum development framework. <br>
 * It contains a brief description of a User use case, followed by a list of Task objects to implement
 * this. <br>
 * The tasks are divided into four groups: To Do, In Process, To Verify, Done. <br>
 * Stories may be marked as completed once all tasks are complete.
 * Once this happens, no changes are allowed to the story.
 */
class Story {
  public:     
    Story(int id, std::string description);
    void createTask(int id, std::string description, bool load);
    void loadTask(int id, std::string description, int column);
    void moveTask(int id, int destination, bool load);
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
 *  \brief Representation the whole Scrum task board.
 */

class Board {
  public:
    void listStories(void);
    void createStory(int id, std::string description);
    void completeStory(int id);
    void loadBoard(std::string fileName);
    void deleteStory(int storyId);
    void exit(std::string filename);
    void createTask(int storyId, int id, std::string description);
    void listTasks(int storyId);
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


