#include <boost/serialization/serialization.hpp>
#include <boost/tokenizer.hpp>
#include <filesystem>
#include <sstream>
#include <unistd.h>

#include "fs.hpp"

FS::FS() {
        /*
         * The root directory doesn't have a parent directory,
         * therefore we use nullptr.
         */
        this->root_dir = Directory(ROOT, filetype::FSDIRECTORY);
        this->root_dir.setAbsolutePath("/");

        this->current_path = this->root_dir.getAbsolutePath();
        this->current_dir = &this->root_dir;
        this->command = commands::DEFAULT;

        /*
         * Check to see if a previous filesystem save file exists,
         * and if so load it.
         */
        if (std::filesystem::exists(SAVEFILE)) {
                this->restoreState(this->root_dir, SAVEFILE);
        }
}

void FS::mainLoop() {
        while (this->command != commands::EXIT) {
                std::cout << "\u001b[1m\033[32mfile-system\033[0m:"
                          << "\u001b[1m\033[34m" << this->current_path
                          << "\033[0m$ ";
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

        /*
         * Get the first word of the unparsed string and check if it's a valid
         * command. If it's not we dont need to continue with parsing.
         */
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
                this->changeDir();
                break;
        case commands::LS:
                this->listDirContents();
                break;
        case commands::MKDIR:
                this->createObject(filetype::FSDIRECTORY);
                break;
        case commands::TOUCH:
                this->createObject(filetype::FSFILE);
                break;
        case commands::RM:
                this->deleteObject();
                break;
        case commands::MV:
                this->moveObject();
                break;
        case commands::CP:
                this->copyObject();
                break;
        case commands::CAT:
                this->printContents();
                break;
        case commands::EXIT:
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

void FS::createObject(filetype filetype) {
        /*
         * Always check if sufficient arguments are given to the command before
         * we proceed.
         */
        if (this->command_arguments.empty()) {
                std::cerr << this->fetchCommand() << ": missing operand"
                          << std::endl;
                return;
        }

        int ret;
        std::string obj_name;
        // Find the path that the object will be created.
        Directory *temp_dir =
            this->findDirPath(this->command_arguments.front(), obj_name);
        if (temp_dir == nullptr) {
                return;
        }

        ret = temp_dir->checkObjName(obj_name);
        if (ret == -1) {
                std::cerr << this->fetchCommand() << ": cannot create object ‘"
                          << obj_name << "’: File name too long" << std::endl;
                return;
        } else if (ret == -2) {
                std::cerr << this->fetchCommand() << ": cannot create object ‘"
                          << obj_name << "’: File exists" << std::endl;
                return;
        }

        if (filetype == filetype::FSDIRECTORY) {
                std::unique_ptr<Directory> dir =
                    std::make_unique<Directory>(obj_name, filetype, temp_dir);
                temp_dir->insertContent(std::move(dir));
        } else {
                std::unique_ptr<File> file =
                    std::make_unique<File>(obj_name, filetype, temp_dir);
                temp_dir->insertContent(std::move(file));
        }
}

void FS::deleteObject() {
        /*
         * Always check if sufficient arguments are given to the command before
         * we proceed.
         */
        if (this->command_arguments.empty()) {
                std::cerr << this->fetchCommand() << ": missing operand"
                          << std::endl;
                return;
        }

        int ret;
        std::string obj_name;
        // Find the path that the object will be deleted.
        Directory *temp_dir =
            this->findDirPath(this->command_arguments.front(), obj_name);
        if (temp_dir == nullptr) {
                return;
        }
        temp_dir->removeContent(obj_name);
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
                                if (key == *it &&
                                    value->getType() == filetype::FSDIRECTORY) {
                                        temp_dir = (Directory *)value.get();
                                        found = true;
                                        break;
                                }
                        }
                        if (!found) {
                                std::cerr << this->fetchCommand() << ": "
                                          << path_to_dir
                                          << ": No such file or directory"
                                          << std::endl;
                                return nullptr;
                        }
                }
        }

        return temp_dir;
}

Directory *FS::findDirPath(std::string path_to_dir) {
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

        for (auto it = tok.begin(); it != tok.end(); ++it) {
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
                                if (key == *it &&
                                    value->getType() == filetype::FSDIRECTORY) {
                                        temp_dir = (Directory *)value.get();
                                        found = true;
                                        break;
                                }
                        }
                        if (!found) {
                                std::cerr << this->fetchCommand() << ": "
                                          << path_to_dir
                                          << ": No such directory" << std::endl;
                                return nullptr;
                        }
                }
        }

        return temp_dir;
}

void FS::changeDir() {
        // if the command argument is empty, we just change to root directory.
        if (this->command_arguments.empty()) {
                this->current_path = this->root_dir.getAbsolutePath();
                this->current_dir = &this->root_dir;
                return;
        }

        std::string path = this->command_arguments.front();
        Directory *target_dir = findDirPath(path);
        if (target_dir == nullptr) {
                return;
        }
        this->current_path = target_dir->getAbsolutePath();
        this->current_dir = target_dir;
}

std::string FS::fetchCommand() {
        switch (this->command) {
        case commands::CD:
                return "cd";
        case commands::LS:
                return "ls";
        case commands::MKDIR:
                return "mkdir";
        case commands::TOUCH:
                return "touch";
        case commands::RM:
                return "rm";
        case commands::MV:
                return "mv";
        case commands::CP:
                return "cp";
        case commands::CAT:
                return "cat";
        case commands::EXIT:
                return "exit";
        default:
                return "default";
        }
}

void FS::listDirContents() {
        if (this->command_arguments.empty()) {
                this->current_dir->listContents();
                return;
        } else {
                std::string path_to_dir = command_arguments.front();
                Directory *dir = findDirPath(path_to_dir);
                if (dir == nullptr) {
                        return;
                } else {
                        dir->listContents();
                }
                return;
        }
}

void FS::printContents() {
        /*
         * Always check if sufficient arguments are given to the command before
         * we proceed.
         */
        if (this->command_arguments.empty()) {
                std::cerr << this->fetchCommand() << ": missing operand"
                          << std::endl;
                return;
        }

        int ret;
        std::string file_name;
        // Find the path of the file
        Directory *temp_dir =
            this->findDirPath(this->command_arguments.front(), file_name);
        if (temp_dir == nullptr) {
                return;
        }
        for (auto &[key, value] : temp_dir->getContents()) {
                if (key == file_name) {
                        if (value->getType() == filetype::FSFILE) {
                                ((File *)value.get())->printContent();
                        } else {
                                std::cerr << this->fetchCommand() << ": "
                                          << this->command_arguments.front()
                                          << ": Is a directory" << std::endl;
                        }
                        break;
                }
        }
}

void FS::moveObject() {
        /*
         * Always check if sufficient arguments are given to the command before
         * we proceed.
         */
        if (this->command_arguments.empty()) {
                std::cerr << this->fetchCommand() << ": missing file operand"
                          << std::endl;
                return;
        } else if (this->command_arguments.size() == 1) {
                std::cerr << this->fetchCommand()
                          << ": missing destination operand after '"
                          << this->command_arguments.front() << "'"
                          << std::endl;
                return;
        }

        std::string src_object_name;
        /*
         * Find the parent directory of where our object is located,
         * and the objects name.
         */
        Directory *source_dir =
            this->findDirPath(this->command_arguments.front(), src_object_name);
        if (source_dir == nullptr) {
                return;
        }
        Directory *dest_dir = this->findDirPath(this->command_arguments.back());
        if (dest_dir == nullptr) {
                return;
        }

        /*
         * If the directories are the same, we attempt to move something in
         * place, therefore an error is produced.
         */
        if (source_dir == dest_dir) {
                /*
                 * This is to ensure that the path appear
                 * correctly for the user
                 */
                if (this->command_arguments.back().back() == '/') {
                        this->command_arguments.back().pop_back();
                }
                std::cerr << "mv: '" << this->command_arguments.front()
                          << "' and '" << this->command_arguments.back() << "/"
                          << src_object_name << "' are the same file"
                          << std::endl;
                return;
        }

        source_dir->moveContent(dest_dir, src_object_name);
}

void FS::copyObject() {
        /*
         * Always check if sufficient arguments are given to the command before
         * we proceed.
         */
        if (this->command_arguments.empty()) {
                std::cerr << this->fetchCommand() << ": missing file operand"
                          << std::endl;
                return;
        } else if (this->command_arguments.size() == 1) {
                std::cerr << this->fetchCommand()
                          << ": missing destination operand after '"
                          << this->command_arguments.front() << "'"
                          << std::endl;
                return;
        }

        std::string src_object_name;

        /*
         * Find the parent directory of where our object is located,
         * and the objects name.
         */
        Directory *source_dir =
            this->findDirPath(this->command_arguments.front(), src_object_name);
        if (source_dir == nullptr) {
                return;
        }
        Directory *dest_dir = this->findDirPath(this->command_arguments.back());
        if (dest_dir == nullptr) {
                return;
        }

        /*
         * If the directories are the same, we attempt to move something in
         * place, therefore an error is produced.
         */
        if (source_dir == dest_dir) {

                /*
                 * This is to ensure that the path appear
                 * correctly for the user
                 */
                if (this->command_arguments.back().back() == '/') {
                        this->command_arguments.back().pop_back();
                }
                std::cerr << "cp: '" << this->command_arguments.front()
                          << "' and '" << this->command_arguments.back() << "/"
                          << src_object_name << "' are the same file"
                          << std::endl;
                return;
        }

        source_dir->copyContent(dest_dir, src_object_name);
}
