#include "dir.hpp"
#include "file-object.hpp"

Directory::Directory(std::string name, filetype type) : FileObject(name, type) {
        time_t now = time(0);
        this->date_of_creation = ctime(&now);
        this->num_of_contents = 0;
        this->size_of_contents = 0;
};

Directory::Directory(std::string name, filetype type, Directory *parent_dir)
    : FileObject(name, type, parent_dir) {
        time_t now = time(0);
        this->date_of_creation = ctime(&now);
        this->num_of_contents = 0;
        this->size_of_contents = 0;
};

FileObject *Directory::findOneContent(std::string name) {
        auto it = this->contents.find(name);
        if (it == contents.end()) {
                return nullptr;
        } else
                return it->second.get();
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
        } else {
                this->num_of_contents--;
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

void Directory::moveContent(Directory *dest_dir, std::string obj_name) {
        Directory *temp_dir = dest_dir;

        /*
         * First find the object that is about to be moved. If not, produce an
         * error. Then we iterate through the path of the DEST directory and
         * compare the absolute path of our object to each directory in our
         * path. If the absolute paths are equal that means that an attempt to
         * move a directory inside itself was made, therefore we produce an
         * error and return.
         */
        auto it = this->contents.find(obj_name);
        if (it == this->contents.end()) {
                std::cerr << "mv: cannot stat '" << obj_name
                          << "': No such file or directory" << std::endl;
                return;
        }
        while (temp_dir != nullptr) {
                if (it->second->getAbsolutePath() ==
                    temp_dir->getAbsolutePath()) {
                        std::cerr << "mv: cannot move '" << obj_name
                                  << "' to a subdirectory of itself"
                                  << std::endl;
                        return;
                }
                temp_dir = temp_dir->getParentDir();
        }

        if (it->second->getType() == filetype::FSDIRECTORY) {
                this->moveDirectory(dest_dir, obj_name);
        } else {
                this->moveFile(dest_dir, obj_name);
        }
}

void Directory::moveDirectory(Directory *dest_dir, std::string dir_name) {
        std::unordered_map<std::string, std::unique_ptr<FileObject>> &contents =
            dest_dir->getContents();

        // We have already checked if the source dir has the directory. Now
        // check for dest_dir.
        auto source_dir_it = this->contents.find(dir_name);
        auto dest_dir_it = contents.find(dir_name);

        /*
         * There are 4 possible cases that are covered here:
         * 1. We move our source directory into a target directory, and we have
         * 0 conflicts.
         * 2. We move the source directory into a target directory, and our
         * target has a file with the same name, therefore we produce an error.
         * 3. We move our source directory into a target directory, and out
         * target has a directory with the same name. There are 2 possible cases
         * here: 3a. The directory inside the target is empty, therefore we
         * replace it. 3b. The directory inside the target is not empty,
         * therefore we produce an error.
         */
        if (dest_dir_it == contents.end()) {
                source_dir_it->second->setParentDir(dest_dir);
                source_dir_it->second->setAbsolutePath(
                    dest_dir->getAbsolutePath(), dir_name);

                /*
                 * Recursively change all the absolute paths and parent
                 * directories of the contents of the directory about to
                 * be moved.
                 */
                ((Directory *)source_dir_it->second.get())
                    ->changeAbsPathOfContentDirs();
                dest_dir->insertContent(std::move(source_dir_it->second));
                this->removeContent(dir_name);
        } else if (dest_dir_it->second->getType() == filetype::FSFILE) {
                std::cerr << "mv: cannot overwrite non-directory '"
                          << dest_dir_it->second->getParentDir()->getName()
                          << "/" << dest_dir_it->second->getName()
                          << "' with directory '" << dir_name << "'"
                          << std::endl;
                return;
        } else if (dest_dir_it->second->getType() == filetype::FSDIRECTORY) {
                /*
                 * We need to use the 'get()' method to cast to `Directory *`,
                 * so we can use `getContents()`.
                 */
                if (((Directory *)dest_dir_it->second.get())
                        ->getContents()
                        .empty()) {
                        dest_dir->removeContent(dir_name);
                        source_dir_it->second->setParentDir(dest_dir);
                        source_dir_it->second->setAbsolutePath(
                            dest_dir->getAbsolutePath(), dir_name);

                        /*
                         * Recursively change all the absolute paths and parent
                         * directories of the contents of the directory about to
                         * be moved.
                         */
                        ((Directory *)source_dir_it->second.get())
                            ->changeAbsPathOfContentDirs();
                        dest_dir->insertContent(
                            std::move(source_dir_it->second));
                        this->removeContent(dir_name);
                } else {
                        std::cerr
                            << "mv: cannot move '" << dir_name << "' to '"
                            << dest_dir_it->second->getParentDir()->getName()
                            << "/" << dest_dir_it->second->getName()
                            << "': Directory not empty" << std::endl;
                        return;
                }
        }
}

void Directory::moveFile(Directory *dest_dir, std::string file_name) {
        std::unordered_map<std::string, std::unique_ptr<FileObject>> &contents =
            dest_dir->getContents();

        // We have already checked if the source dir has the file. Now check for
        // dest_dir.
        auto source_file_it = this->contents.find(file_name);
        auto dest_dir_it = contents.find(file_name);

        /*
         * If the file does not exist in our target directory, then we just move
         * it there. If a file with the same name does exist, it gets replaced.
         * If a directory with the same name exist we produce an error.
         */
        if (dest_dir_it == contents.end()) {
                source_file_it->second->setParentDir(dest_dir);
                source_file_it->second->setAbsolutePath(
                    dest_dir->getAbsolutePath(), file_name);
                dest_dir->insertContent(std::move(source_file_it->second));
                this->removeContent(file_name);
        } else if (dest_dir_it->second->getType() == filetype::FSDIRECTORY) {
                std::cerr << "mv: cannot overwrite directory '"
                          << dest_dir_it->second->getParentDir()->getName()
                          << "/" << dest_dir_it->second->getName()
                          << "' with non-directory" << std::endl;
                return;
        } else if (dest_dir_it->second->getType() == filetype::FSFILE) {
                dest_dir->removeContent(file_name);
                source_file_it->second->setParentDir(dest_dir);
                source_file_it->second->setAbsolutePath(
                    dest_dir->getAbsolutePath(), file_name);
                dest_dir->insertContent(std::move(source_file_it->second));
                this->removeContent(file_name);
        }
}

/*
 * This method changes the absolute path and the parent directories
 * on all directories  in `this->content` recursively.
 */
void Directory::changeAbsPathOfContentDirs() {
        for (auto &[key, value] : this->contents) {
                value->setAbsolutePath(value->getParentDir()->getAbsolutePath(),
                                       value->getName());
                if (value->getType() == filetype::FSDIRECTORY) {
                        ((Directory *)value.get())
                            ->changeAbsPathOfContentDirs();
                }
        }
}

void Directory::copyContent(Directory *dest_dir, std::string obj_name) {
        /* Find the object inside our source directory, and if found check if
         * it is of type FSFILE. If not produce an error.
         */
        auto it_source = this->contents.find(obj_name);
        if (it_source == this->contents.end()) {
                std::cerr << "cp: cannot stat '" << obj_name
                          << "': No such file" << std::endl;
                return;
        } else if (it_source->second->getType() == filetype::FSDIRECTORY) {
                std::cerr << "cp: cannot copy directory" << std::endl;
                return;
        }

        auto it_dest = dest_dir->getContents().find(obj_name);
        if (it_dest == dest_dir->getContents().end()) {
                std::unique_ptr<File> file = std::make_unique<File>(
                    obj_name, filetype::FSFILE, dest_dir);
                file->setContent(
                    ((File *)it_source->second.get())->getContent());
                dest_dir->insertContent(std::move(file));
        } else if (it_dest->second->getType() == filetype::FSDIRECTORY) {
                std::cerr << "cp: cannot overwrite directory '"
                          << it_dest->second->getParentDir()->getName() << "/"
                          << it_dest->second->getName()
                          << "' with non-directory" << std::endl;
        } else {
                ((File *)it_dest->second.get())
                    ->setContent(
                        ((File *)it_source->second.get())->getContent());
        }
}
