#include "dir.hpp"

Directory::Directory(std::string name, filetype type) : name(name), type(type) {
  time_t now = time(0);
  this->date_of_creation = ctime(&now);
  this->num_of_contents = 0;
  this->size_of_contents = 0;
};

std::string Directory::getName() { return this->name; }

void Directory::setName(std::string name) { this->name = name; }

std::string Directory::getDate() { return this->date_of_creation; }

filetype Directory::getType() { return this->type; }

File *Directory::getContent(std::string name) {
  std::unordered_map<std::string, File *>::iterator it =
      this->contents.find(name);
  if (it == contents.end()) {
    std::cout << "file not found" << std::endl;
    return NULL;
  } else
    return it->second;
};

void Directory::insertContent(std::string name, File *file) {
  this->contents.insert({name, file});
  this->num_of_contents++;
}

void Directory::listContents() {
  for (auto it = this->contents.begin(); it != this->contents.end(); it++) {
    std::cout << it->first << std::endl;
  }
}

int Directory::getNumContents() { return this->num_of_contents; }

int Directory::getSizeOfContents() { return this->size_of_contents; }
