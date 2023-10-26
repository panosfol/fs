#ifndef DIR_H_NAME
#define DIR_H_NAME

#include <unordered_map>

#include "file.hpp"

class Directory {
protected:
  std::string name;
  const char *date_of_creation;
  const filetype type;
  std::unordered_map<std::string, File *> contents;
  int num_of_contents;
  int size_of_contents;

public:
  Directory(std::string, filetype type);

  std::string getName();
  void setName(std::string);

  std::string getDate();

  filetype getType();

  File *getContent(std::string);
  void insertContent(std::string, File *);

  void listContents();

  int getNumContents();
  int getSizeOfContents();
};
#endif
