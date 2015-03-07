
class ex_noStory {
	public: 
	  ex_noStory(int _id) : id(_id) {};
    int id;
};

class ex_noTask {
	public: 
	  ex_noTask(int _id) : id(_id) {};
    int id;
};

class ex_column {
	public: 
	  ex_column(int _id) : id(_id) {};
    int id;
};

class ex_integrity {
	public: 
    ex_integrity() {msg = "Task in invalid column";};
	  std::string msg;
};

class ex_file {
	public: 
	  ex_file(std::string _file) : file(_file) {};
    std::string file;
};

class ex_storyExists {
	public: 
	  ex_storyExists(int _id) : id(_id) {};
    int id;
};

class ex_taskExists {
	public: 
	  ex_taskExists(int _id) : id(_id) {};
    int id;
};

class ex_undefArg {
	public: 
	  ex_undefArg(std::string _command) : command(_command) {};
    std::string command;
};

class ex_numArg {
	public: 
	  ex_numArg(std::string _command) : command(_command) {};
    std::string command;
};

class ex_undefCmd {
	public: 
	  ex_undefCmd(std::string _command) : command(_command) {};
    std::string command;
};

