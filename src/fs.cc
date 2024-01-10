#include <boost/serialization/serialization.hpp>
#include <boost/tokenizer.hpp>
#include <filesystem>
#include <sstream>
#include <unistd.h>

#include "fs.hpp"

FS::FS() {
        // The root directory doesn't have a parent directory, therefore we use
        // nullptr.
        this->root_dir = Directory(ROOT, filetype::FSDIRECTORY, nullptr, false);
        this->root_dir.setAbsolutePath("/");

        this->current_path = this->root_dir.getAbsolutePath();
        this->current_dir = &this->root_dir;
        this->command = commands::DEFAULT;

        // Check to see if a previous filesystem save file exists, and if so
        // load it.
        if (std::filesystem::exists(SAVEFILE)) {
                this->restoreState(this->root_dir, SAVEFILE);
        }
}

void FS::mainLoop() {
        while (this->command != commands::EXIT) {
                std::cout << "file-system:" << this->current_path << "$ ";
                this->command = commands::DEFAULT;
                if (this->parseCommand()) {
                        this->executeCommand();
                }
        }
        this->saveState(this->root_dir, SAVEFILE);
}

bool FS::parseCommand() {
        std::string unparsed_command;
        std::string word;

        std::getline(std::cin, unparsed_command);
        std::stringstream ss(unparsed_command);

        // Get the first word of the unparsed string and check if it's a valid
        // command. If it's not we dont need to continue with parsing.
        ss >> word;

        if (word == "cd") {
                this->command = commands::CD;
        } else if (word == "ls") {
                this->command = commands::LS;
        } else if (word == "mkdir") {
                this->command = commands::MKDIR;
        } else if (word == "touch") {
                this->command = commands::TOUCH;
        } else if (word == "rm") {
                this->command = commands::RM;
        } else if (word == "mv") {
                this->command = commands::MV;
        } else if (word == "cp") {
                this->command = commands::CP;
        } else if (word == "cat") {
                this->command = commands::CAT;
        } else if (word == "find") {
                this->command = commands::FIND;
        } else if (word == "info") {
                this->command = commands::INFO;
        } else if (word == "help") {
                this->command = commands::HELP;
        } else if (word == "exit") {
                this->command = commands::EXIT;
        } else {
                std::cerr << "Command \'" << word << "\' not found"
                          << std::endl;
                this->command = commands::ERROR;
                return false;
        }

        // Clear the vector to hold the correct arguments.
        this->command_arguments.clear();
        while (ss >> word) {
                this->command_arguments.push_back(word);
        }

        return true;
}

void FS::executeCommand() {
        switch (this->command) {
        case commands::CD:
                break;
        case commands::LS:
                this->current_dir->listContents();
                break;
        case commands::MKDIR:
                this->createDirectory();
                break;
        case commands::TOUCH:
                break;
        case commands::RM:
                break;
        case commands::MV:
                break;
        case commands::CP:
                break;
        case commands::CAT:
                break;
        case commands::FIND:
                break;
        case commands::INFO:
                break;
        case commands::EXIT:
                break;
        case commands::HELP:
                break;
        case commands::DEFAULT:
                break;
        }
}

void FS::saveState(const Directory &s, const char *filename) {
        // make an archive
        std::ofstream ofs(filename);
        assert(ofs.good());
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_NVP(s);
}

void FS::restoreState(Directory &s, const char *filename) {
        // open the archive
        std::ifstream ifs(filename);
        assert(ifs.good());
        boost::archive::xml_iarchive ia(ifs);
        // restore the filesystem from the archive
        ia >> BOOST_NVP(s);
}

void FS::createDirectory() {
        // Always check if sufficient arguments are given to the command before
        // we proceed.
        if (this->command_arguments.empty()) {
                std::cerr << "mkdir: missing operand" << std::endl;
                return;
        }

        std::string dir_name;
        // Find the path that the directory will be created.
        Directory *temp_dir =
            this->findDirPath(this->command_arguments.front(), dir_name);
        if (temp_dir == nullptr) {
                return;
        }
        if (temp_dir->checkDirName(dir_name) != 0) {
                return;
        }

        std::unique_ptr<Directory> dir = std::make_unique<Directory>(
            dir_name, filetype::FSDIRECTORY, temp_dir, true);
        dir->setAbsolutePath(this->current_path, dir_name);
        temp_dir->insertContent(std::move(dir));
}

Directory *FS::findDirPath(std::string path_to_dir, std::string &name) {
        bool found;
        Directory *temp_dir;
        std::vector<std::string> temp_vector;
        boost::char_separator<char> sep("/");
        boost::tokenizer tok(path_to_dir, sep);

        /*
         * If the given path starts with '/' we should start searching from the
         * root directory, if not then we start from the current.
         */
        if (path_to_dir.at(0) == '/') {
                temp_dir = (Directory *)&this->root_dir;
        } else {
                temp_dir = (Directory *)this->current_dir;
        }

        /*
         * Get the elements from the tokenizer in a temporary vector.
         * This is needed so we can access the last element of the sequence,
         * which is the name that will be given to the newly created directory.
         * After getting the name, we remove it from the vector, so we can
         * iterate through the path to find the (possible) parent directory of
         * the directory we want to create.
         */
        for (auto it = tok.begin(); it != tok.end(); ++it) {
                temp_vector.push_back(*it);
        }
        name = temp_vector.back();
        temp_vector.pop_back();

        for (auto it = temp_vector.begin(); it != temp_vector.end(); ++it) {
                if (*it == "..") {
                        Directory *parent_dir = temp_dir->getParentDir();
                        if (parent_dir == nullptr) {
                                continue;
                        } else {
                                temp_dir = parent_dir;
                                continue;
                        }
                } else if (*it == ".") {
                        continue;
                } else {
                        found = false;
                        for (auto &[key, value] : temp_dir->getContents()) {
                                if (key == *it) {
                                        temp_dir = (Directory *)value.get();
                                        found = true;
                                        break;
                                }
                        }
                        if (!found) {
                                std::cerr << "mkdir: " << path_to_dir
                                          << " No such file or directory"
                                          << std::endl;
                                return nullptr;
                        }
                }
        }

        return temp_dir;
}
