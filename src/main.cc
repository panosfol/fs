#include <iostream>

#include "dir.hpp"
#include "file.hpp"
#include "fs.hpp"

int main() {
  FS *fs = new FS();

  fs->main_loop();
  delete (fs);

  return 0;
}
