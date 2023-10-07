#include <sstream>

#include "root.hpp"

Root::Root() { this->current_path = "~/home"; }

void Root::main_loop() {
  while (1) {
    std::cout << "file-system:" << this->current_path << "$ ";
    if (this->parse_string()) {
      execute_command(this->command);
    }
  }
}

bool Root::parse_string() {
  std::string unparsed_string;
  std::string word;

  std::getline(std::cin, unparsed_string);
  std::stringstream ss(unparsed_string);

  // Get the first word of the unparsed string and check if its a valid command.
  // If it's not we dont need to continue with parsing.
  ss >> word;

  if (word == "cd") {
    this->command = commands::CD;
  } else if (word == "ls") {
    this->command = commands::LS;
  } else if (word == "mkdir") {
    this->command = commands::MKDIR;
  } else if (word == "touch") {
    this->command = commands::TOUCH;
  } else if (word == "rm") {
    this->command = commands::RM;
  } else if (word == "mv") {
    this->command = commands::MV;
  } else if (word == "cp") {
    this->command = commands::CP;
  } else if (word == "cat") {
    this->command = commands::CAT;
  } else if (word == "find") {
    this->command = commands::FIND;
  } else if (word == "info") {
    this->command = commands::INFO;
  } else if (word == "help") {
    this->command = commands::HELP;
  } else if (word == "exit") {
    this->command = commands::EXIT;
  } else {
    std::cerr << "Command \'" << word << "\' not found" << std::endl;
    this->command = commands::ERROR;
    return false;
  }

  this->user_command.push_back(word);
  while (ss >> word) {
    this->user_command.push_back(word);
  }

  return true;
}

// This is just the basic structure until the commands are fully implemented
void Root::execute_command(commands command) {
  switch (command) {
  case commands::CD:
    break;
  case commands::LS:
    break;
  case commands::MKDIR:
    break;
  case commands::TOUCH:
    break;
  case commands::RM:
    break;
  case commands::MV:
    break;
  case commands::CP:
    break;
  case commands::CAT:
    break;
  case commands::FIND:
    break;
  case commands::INFO:
    break;
  case commands::EXIT:
    break;
  case commands::HELP:
    break;
  }
}
