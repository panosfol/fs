#include "dir.hpp"
#include "file-object.hpp"

Directory::Directory(std::string name, filetype type, Directory *parent_dir)
    : FileObject(name, type, parent_dir) {
        time_t now = time(0);
        this->date_of_creation = ctime(&now);
        this->num_of_contents = 0;
        this->size_of_contents = 0;
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

void Directory::insertContent(std::unique_ptr<FileObject> object) {
        auto [it, ok] =
            this->contents.emplace(object->getName(), std::move(object));
        if (ok) {
                this->num_of_contents++;
        }
}

void Directory::removeContent(std::string name) {
        if (!this->contents.erase(name)) {
                std::cerr << "rm: cannot remove '" << name
                          << "': No such file or directory" << std::endl;
        }
}

void Directory::listContents() {
        for (auto it = this->contents.begin(); it != this->contents.end();
             it++) {
                if (it->second->getType() == filetype::FSDIRECTORY) {
                        std::cout << "\u001b[1m\033[34m"
                                  << it->second->getName() << "\033[0m"
                                  << std::endl;
                } else {
                        std::cout << it->second->getName() << std::endl;
                }
        }
}

int Directory::getNumContents() { return this->num_of_contents; }

int Directory::getSizeOfContents() { return this->size_of_contents; }

/*
 *  checkObjName
 *  Check if the given string is appropriate for an object
 *  in accordance to the Linux specification
 *
 * Returns:
 *  0 Success
 * -1 Error - Name too long
 * -2 Error - Object already exists
 */
int Directory::checkObjName(std::string name) {
        // 255 is the maximum length of a name for a directory that Linux allows
        if (name.length() > 255) {
                return -1;
        }

        // Check if the directory already exists
        for (auto &[key, value] : this->contents) {
                if (name == key) {
                        return -2;
                }
        }
        return 0;
};
