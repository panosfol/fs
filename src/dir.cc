#include "dir.hpp"

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
}

void Directory::listContents() {
  for (auto it = this->contents.begin(); it != this->contents.end(); it++) {
    std::cout << it->first << std::endl;
  }
}
