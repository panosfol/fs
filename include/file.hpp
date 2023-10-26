#ifndef FILE_H_NAME
#define FILE_H_NAME

#include <iostream>

#include "file-object.hpp"

class File : public FileObject {
protected:
  std::string content;
  int size;

public:
  File(std::string, filetype file_type);

  void setSize(int);
  int getSize();
  // TODO methods for creating and reading content
};
#endif
