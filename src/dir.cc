#include "dir.hpp"

Directory::Directory(std::string name, filetype type) : FileObject(name, type) {
  time_t now = time(0);
  this->date_of_creation = ctime(&now);
  this->num_of_contents = 0;
  this->size_of_contents = 0;
};

FileObject *Directory::findOneContent(std::string name) {
  std::unordered_map<std::string, FileObject *>::iterator it =
      this->contents.find(name);
  if (it == contents.end()) {
    std::cout << "file not found" << std::endl;
    return NULL;
  } else
    return it->second;
};

std::unordered_map<std::string, FileObject *> Directory::getContents() {
  return this->contents;
}

void Directory::insertContent(std::string name, FileObject *file) {
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
