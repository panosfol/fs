#ifndef FS_H_NAME
#define FS_H_NAME

#include <boost/archive/tmpdir.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>
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

class FS {
public:
  FS();

  void main_loop();

  bool parse_command();
  void execute_command(commands);

  std::string file_search();
  std::string file_delete(std::string);

  void save_state(const Directory &s, const char *filename);
  void restore_state(Directory &s, const char *filename);
  std::vector<std::string> user_command;
  commands command;

  std::string current_path;
  std::string user_input;

  Directory *current_dir;
};
#endif
