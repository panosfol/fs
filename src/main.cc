#include <boost/archive/tmpdir.hpp>
#include <boost/serialization/export.hpp>
#include <fstream>
#include <iostream>

#include "dir.hpp"
#include "file.hpp"
#include "fs.hpp"

BOOST_SERIALIZATION_ASSUME_ABSTRACT(FileObject)
BOOST_CLASS_EXPORT(File)
BOOST_CLASS_EXPORT(Directory)

int main() {
        std::unique_ptr<FS> fs = std::make_unique<FS>();
        fs->mainLoop();

        return 0;
}
