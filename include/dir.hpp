#ifndef DIR_H_NAME
#define DIR_H_NAME

#include <unordered_map>

#include "file-object.hpp"

class Directory : public FileObject {
protected:
  std::unordered_map<std::string, FileObject *> contents;
  int num_of_contents;
  int size_of_contents;

public:
  Directory(std::string, filetype type);

  FileObject *findOneContent(std::string name);
  std::unordered_map<std::string, FileObject *> getContents();
  void insertContent(std::string, FileObject *);

  void listContents();

  int getNumContents();
  int getSizeOfContents();
};
#endif
