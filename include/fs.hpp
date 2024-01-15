#ifndef FS_H_NAME
#define FS_H_NAME

#include <boost/archive/tmpdir.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>

#include "dir.hpp"

#define SAVEFILE "fs_save.xml"

// The commands that are currently supported
enum commands {
        CD,
        LS,
        MKDIR,
        TOUCH,
        RM,
        MV,
        CP,
        CAT,
        FIND,
        INFO,
        EXIT,
        HELP,
        ERROR,
        DEFAULT
};

class FS {
public:
        FS();

        void mainLoop();

        bool parseCommand();
        void executeCommand();
        std::string fetchCommand();

        std::string fileSearch();
        std::string fileDelete(std::string);

        void saveState(const Directory &s, const char *);
        void restoreState(Directory &s, const char *);

        void createDirectory();

        /*
         * This is used for creating a new Directory. After parsing the path, we
         * save the name in the given variable so we can create the Directory
         * after.
         */
        Directory *findDirPath(std::string path_to_dir, std::string &name);

        /*
         * This is used for generally finding the path, and this is what will be
         * called in the majority of commands. Only the path is needed, which
         * will be parsed and the directory found will be returned.
         */
        Directory *findDirPath(std::string path_to_dir);
        void changeDir();

        std::vector<std::string> command_arguments;
        commands command;

        std::string current_path;
        std::string user_input;

        Directory *current_dir;
        Directory root_dir;
};
#endif
