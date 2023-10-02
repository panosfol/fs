#ifndef ROOT_H_NAME
#define ROOT_H_NAME

#include "dir.hpp"

class Root : public Directory {
public:
  std::string file_search();
  std::string file_delete(std::string);
};
#endif
