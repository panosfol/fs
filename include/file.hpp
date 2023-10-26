#ifndef FILE_H_NAME
#define FILE_H_NAME

#include <iostream>

enum filetype { FSFILE, FSDIRECTORY, ROOT };

class File {
protected:
  std::string name;
  std::string content;
  const char *date_of_creation;
  const filetype type;
  int size;

public:
  File(std::string, filetype file_type);

  std::string getName();
  void setName(std::string);

  std::string getDate();

  filetype getType();

  void setSize(int);
  int getSize();
  // TODO methods for creating and reading content
};
#endif
