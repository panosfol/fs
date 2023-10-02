#ifndef FILE_H_NAME
#define FILE_H_NAME

#include <iostream>

enum filetype { FSFILE, FSDIRECTORY, ROOT };

class File {
protected:
  std::string name;
  std::string content;
  const time_t date_of_creation;
  const filetype type;
  int size;

public:
  File(std::string, time_t date, filetype file_type);

  std::string getName();
  void setName(std::string);

  time_t getDate();

  filetype getType();

  void setSize(int);
  int getSize();
  // TODO methods for creating and reading content
};
#endif
