#include <vector>
#include <string>

#define TODO 0
#define IN_PROCESS 1
#define TO_VERIFY 2
#define DONE 3

#define NO_ERROR 0
#define ERROR_NO_STORY 1
#define ERROR_NO_TASK 2
#define ERROR_COLUMN 3
#define ERROR_INTEGRITY 4
#define ERROR_FILE 5
#define ERROR_STORY_EXISTS 6
#define ERROR_TASK_EXISTS 7
#define ERROR_UNDEF_ARG 8
#define ERROR_NUM_ARG 9
#define ERROR_UNDEF_CMD 10

extern int error;

class Task {
  public:
    Task(int id, std::string description);
    bool updateDescription(std::string description);
    int getId(void);
    bool storeTask(std::ofstream &file, int storyId, int column);
    std::string getDescription(void);
  private:
    int id;
    std::string description;
};

class Story {
  public:     
    Story(int id, std::string description);
    bool createTask(int id, std::string description);
    bool loadTask(int id, std::string description, int column);
    bool moveTask(int id, int destination);                                
    bool completeStory(void);
    bool isComplete(void);
    bool deleteTask(int id);
    bool updateTask(int id, std::string description);
    bool taskExists(int id);
    bool clearStory(void);
    int getId(void);
    std::string getDescription(void);
    bool storeStory(std::ofstream &file);
    bool listTasks();

  private:
    int id;
    bool complete;  
    std::string description;
    std::vector<std::vector<Task*>> tasks;
    Task* getTask(int id);
};

class Board {
  public:
    bool listStories(void);
    bool createStory(int id, std::string description);
    bool completeStory(int id);
    bool loadBoard(std::string fileName);
    bool deleteStory(int id);
    bool exit(std::string filename);
    bool createTask(int storyId, int id, std::string description);
    bool listTasks(int storyId);
    bool deleteTask(int storyId, int id);
    bool moveTask(int storyId, int id, int column);
    bool updateTask(int storyId, int id, std::string description);

    std::vector<Story*> backlog;
  private:
    bool loadStory(int id, std::string description, bool complete);
    bool loadTask(int storyId, int id, std::string description, int column);
    bool storyExists(int id);
    bool storeBoard(std::string filename); 
    Story* getStory(int id);
    
};
