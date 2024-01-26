#include "file-object.hpp"
#include "dir.hpp"

FileObject::FileObject(std::string name, filetype type)
    : name(name), type(type) {
        this->parent_dir = nullptr;
};

FileObject::FileObject(std::string name, filetype type, Directory *parent_dir)
    : name(name), type(type), parent_dir(parent_dir) {
        this->setAbsolutePath(this->getParentDir()->getAbsolutePath(),
                              this->getName());
};

std::string FileObject::getName() const { return this->name; };

void FileObject::setName(std::string name) { this->name = name; };

/*
 * If the parent directory is the root, then we just need to append
 * the name of the newly created object to our path.
 * If not then we first need to append the '/' character.
 */
void FileObject::setAbsolutePath(std::string path, std::string name) {
        if (this->parent_dir->getName() == ROOT) {
                path.append(name);
        } else {
                path.append("/");
                path.append(name);
        }

        this->absolute_path = path;
}

/*
 * This will be called only when initializing the FS class and it is used
 * only for creating the absolute path of the root directory
 */
void FileObject::setAbsolutePath(std::string path) {
        this->absolute_path = path;
}

std::string FileObject::getAbsolutePath() { return this->absolute_path; }

void FileObject::setParentDir(Directory *parent_dir) {
        this->parent_dir = parent_dir;
}

Directory *FileObject::getParentDir() { return this->parent_dir; }

std::string FileObject::getDate() { return this->date_of_creation; };

filetype FileObject::getType() { return this->type; };
