#include <vector>

class Task {
  public:
    Task(int id, std::string description);
    bool updateDescription(std::string description);
  private:
    int id;
    int column;
    std::string description;
};

class Story {
  public:
    Story(int id, std::string description);
    bool createTask(int id, std::string description);
    bool loadTask(int id, std::string description, int column);
    bool moveTask(int id, int column);                                
    bool advanceTask(Task t);
    bool completeStory(void);
    bool deleteStory(void);
    bool deleteTask(int id);
    bool updateTask(int id);

  private:
    int id;
    bool complete;  
    std::string description;
    Task findTask(int id); 
    std::vector<Task> toDo;
    std::vector<Task> inProcess;
    std::vector<Task> toVerify;
    std::vector<Task> done;
};

class Board {
  public:
    bool listStories(void);
    bool createStory(int id, std::string description);
    bool exit(void);
    bool loadBoard(std::string fileName);
    Story getStory(int id);

  private:
    bool loadStory(int id, std::string description, bool complete);
    std::vector<Story> backlog;
    bool storeBoard();
};

  
