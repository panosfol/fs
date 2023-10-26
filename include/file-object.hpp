#ifndef OBJECT_H_NAME
#define OBJECT_H_NAME

#include <iostream>
#include <unordered_map>

// The different type of files that are supported
enum filetype { FSFILE, FSDIRECTORY, ROOT };

class FileObject {
protected:
  std::string name;
  std::string date_of_creation;
  filetype type;

public:
  FileObject(std::string, filetype);
  FileObject(std::string);

  std::string getName();
  void setName(std::string);

  std::string getDate();

  filetype getType();
};
#endif
