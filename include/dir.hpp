#ifndef DIR_H_NAME
#define DIR_H_NAME

#include <unordered_map>

#include "file.hpp"

class Directory : public File {
protected:
  std::unordered_map<std::string, File *> contents;
  int num_of_contents;
  int size_of_contents;

public:
  Directory(std::string, filetype type);

  File *getContent(std::string);
  void insertContent(std::string, File *);

  void listContents();

  int getNumContents();
  int getSizeOfContents();
};
#endif
