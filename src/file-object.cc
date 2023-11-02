#include "file-object.hpp"

FileObject::FileObject(std::string name, filetype type)
    : name(name), type(type){};

std::string FileObject::getName() const { return this->name; };

void FileObject::setName(std::string name) { this->name = name; };

std::string FileObject::getDate() { return this->date_of_creation; };

filetype FileObject::getType() { return this->type; };
