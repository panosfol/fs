#include "file.hpp"

File::File(std::string name, time_t date, filetype type)
    : name(name), date_of_creation(date), type(type) {}

std::string File::getName() { return this->name; }

void File::setName(std::string name) { this->name = name; }

time_t File::getDate() { return this->date_of_creation; }

filetype File::getType() { return this->type; }

void File::setSize(int size) { this->size = size; }

int File::getSize() { return this->size; }

// TODO methods for content should be added when content-handling is figured out
