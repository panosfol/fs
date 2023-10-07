#include <iostream>

#include "dir.hpp"
#include "file.hpp"
#include "root.hpp"

int main() {
  Root *root = new Root();

  root->main_loop();
  delete (root);

  return 0;
}
