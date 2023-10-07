#ifndef ROOT_H_NAME
#define ROOT_H_NAME

#include <iostream>
#include <unordered_map>
#include <vector>

#include "dir.hpp"

// The commands that are currently supported
enum commands {
  CD,
  LS,
  MKDIR,
  TOUCH,
  RM,
  MV,
  CP,
  CAT,
  FIND,
  INFO,
  EXIT,
  HELP,
  ERROR
};

class Root {
protected:
  std::unordered_map<std::string, File *> contents;

public:
  Root();

  void main_loop();

  bool parse_string();
  void execute_command(commands);

  std::string file_search();
  std::string file_delete(std::string);

  std::vector<std::string> user_command;
  commands command;

  std::string current_path;
  std::string user_input;

  Directory *current_dir;
};
#endif
