#include "file.hpp"

File::File(std::string name, filetype type, Directory *parent_dir)
    : FileObject(name, type, parent_dir) {
        time_t now = time(0);
        this->date_of_creation = ctime(&now);
}

void File::setSize(int size) { this->size = size; }

int File::getSize() { return this->size; }

// TODO methods for content should be added when content-handling is figured out
