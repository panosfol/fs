#ifndef ROOT_H_NAME
#define ROOT_H_NAME

#include <iostream>
#include <unordered_map>
#include <vector>

#include "dir.hpp"

class Root {
protected:
    std::unordered_map<std::string, File *> contents;
public:
  Root();

  void main_loop();

  std::string file_search();
  std::string file_delete(std::string);
  std::vector<std::string> parse_string();

  std::vector<std::string> input;

  std::string current_path;
  std::string user_input;

  Directory *current_dir;
};
#endif
