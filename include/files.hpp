#include <iostream>
#include <unordered_map>

enum type{ FFILE, FDIRECTORY, ROOT };
//enum

class File {
protected:
  std::string name;
  time_t date_of_creation;
  type file_type;
  int size;
};

class Directory : public File{
protected:
  std::unordered_map<std::string, File*> contents;
  int num_of_contents;
  int size_of_contents;
};

class Root : public Directory {
public:
  std::string file_search();
  std::string file_delete(std::string);
};
