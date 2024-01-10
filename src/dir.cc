#include "dir.hpp"

Directory::Directory(std::string name, filetype type, Directory *parent_dir,
                     bool visibility)
    : FileObject(name, type) {
        time_t now = time(0);
        this->date_of_creation = ctime(&now);
        this->num_of_contents = 0;
        this->size_of_contents = 0;
        this->parent_dir = parent_dir;
};

std::unique_ptr<FileObject> Directory::findOneContent(std::string name) {
        std::unordered_map<std::string, std::unique_ptr<FileObject>>::iterator
            it = this->contents.find(name);
        if (it == contents.end()) {
                std::cout << "file not found" << std::endl;
                return NULL;
        } else
                return std::move(it->second);
};

std::unordered_map<std::string, std::unique_ptr<FileObject>> &
Directory::getContents() {
        return this->contents;
}

void Directory::insertContent(std::unique_ptr<Directory> object) {
        auto [it, ok] =
            this->contents.emplace(object->getName(), std::move(object));
        if (ok) {
                this->num_of_contents++;
        }
}

void Directory::listContents() {
        for (auto it = this->contents.begin(); it != this->contents.end();
             it++) {
                std::cout << it->second->getName() << std::endl;
        }
}

int Directory::getNumContents() { return this->num_of_contents; }

int Directory::getSizeOfContents() { return this->size_of_contents; }

/*
 *  checkDirName
 *  Check if the given string is appropriate for a directory
 *  in accordance to the Linux specification
 *
 * Returns:
 *  0 Success
 * -1 Failure - Name not allowed
 *
 */
int Directory::checkDirName(std::string name) {
        // 255 is the maximum length of a name for a directory that Linux allows
        if (name.length() > 255) {
                std::cerr << "mkdir: cannot create directory ‘" << name
                          << "’: File name too long" << std::endl;
                return -1;
        }

        // Check if the directory already exists
        for (auto &[key, value] : this->contents) {
                if (name == key) {
                        std::cerr << "mkdir: cannot create directory ‘" << name
                                  << "’: File exists" << std::endl;
                        return -1;
                }
        }
        return 0;
};
