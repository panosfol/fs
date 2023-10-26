#include "file.hpp"

File::File(std::string name, filetype type) : name(name), type(type) {
  time_t now = time(0);
  this->date_of_creation = ctime(&now);
}

std::string File::getName() { return this->name; }

void File::setName(std::string name) { this->name = name; }

std::string File::getDate() { return this->date_of_creation; }

filetype File::getType() { return this->type; }

void File::setSize(int size) { this->size = size; }

int File::getSize() { return this->size; }

// TODO methods for content should be added when content-handling is figured out
