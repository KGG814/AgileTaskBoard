
/**
 * \brief Exception: Story does not exist
 */
class ex_noStory {
	public: 
	  ex_noStory(int _id) : id(_id) {};
    int id;
};

/**
 * \brief Exception: Task does not exist
 */
class ex_noTask {
	public: 
	  ex_noTask(int _id) : id(_id) {};
    int id;
};

/**
 * \brief Exception: Task already at destination
 */
class ex_column {
	public: 
	  ex_column(int _id) : id(_id) {};
    int id;
};

/**
 * \brief Exception: Task in invalid column
 */
class ex_integrity {
	public: 
    ex_integrity() {msg = "Task in invalid column";};
	  std::string msg;
};

/**
 * \brief Exception: File could not be opened
 */
class ex_file {
	public: 
	  ex_file(std::string _file) : file(_file) {};
    std::string file;
};

/**
 * \brief Exception: Story already exists
 */
class ex_storyExists {
	public: 
	  ex_storyExists(int _id) : id(_id) {};
    int id;
};

/**
 * \brief Exception: Task already exists
 */
class ex_taskExists {
	public: 
	  ex_taskExists(int _id) : id(_id) {};
    int id;
};

/**
 * \brief Exception : Undefined argument
 */
class ex_undefArg {
	public: 
	  ex_undefArg(std::string _command) : command(_command) {};
    std::string command;
};

/**
 * \brief Exception : Not enough arguments
 */
class ex_numArg {
	public: 
	  ex_numArg(std::string _command) : command(_command) {};
    std::string command;
};

/**
 * \brief Exception : Undefined command
 */
class ex_undefCmd {
	public: 
	  ex_undefCmd(std::string _command) : command(_command) {};
    std::string command;
};

/**
 * \brief Exception : Destination undefined
 */
class ex_undefCol {
	public: 
    ex_undefCol() {msg = "Destination not defined";};
	  std::string msg;
};

/**
 * \brief Exception : Task is complete
 */
class ex_taskDone {
	public: 
	  ex_taskDone(int _id) : id(_id) {};
    int id;
};

/**
 * \brief Exception : Tasks must progress through consecutive columns
 */
class ex_progression {
	public: 
    ex_progression() {msg = "Tasks must progress through consecutive columns";};
	  std::string msg;

};

/**
 * \brief Exception : Story has incomplete Tasks
 */
class ex_incompleteTasks {
	public: 
	  ex_incompleteTasks(int _id) : id(_id) {};
    int id;
};

/**
 * \brief Exception : Story is complete
 */
class ex_storyComplete {
	public: 
	  ex_storyComplete(int _id) : id(_id) {};
    int id;
};

