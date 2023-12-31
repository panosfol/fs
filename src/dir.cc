#include "dir.hpp"

Directory::Directory(std::string name, filetype type) : FileObject(name, type) {
  time_t now = time(0);
  this->date_of_creation = ctime(&now);
  this->num_of_contents = 0;
  this->size_of_contents = 0;
};

std::unique_ptr<FileObject> Directory::findOneContent(std::string name) {
  std::unordered_map<std::string, std::unique_ptr<FileObject>>::iterator it =
      this->contents.find(name);
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
  auto [it, ok] = this->contents.emplace(object->getName(), std::move(object));
  if (ok) {
    this->num_of_contents++;
  }
}

void Directory::listContents() {
  for (auto it = this->contents.begin(); it != this->contents.end(); it++) {
    std::cout << it->second->getName() << std::endl;
  }
}

int Directory::getNumContents() { return this->num_of_contents; }

int Directory::getSizeOfContents() { return this->size_of_contents; }
