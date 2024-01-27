#include "file.hpp"

File::File(std::string name, filetype type, Directory *parent_dir)
    : FileObject(name, type, parent_dir) {
        this->writeToFile();
}

void File::writeToFile() {
        std::string newContent;
        std::cout << "Enter new content for file (type 'exit' to finish):"
                  << std::endl;

        std::string line;
        while (std::getline(std::cin, line)) {
                if (line == "exit") {
                        break;
                }
                newContent += line + '\n';
        }

        this->setContent(newContent);
}

void File::printContent() { std::cout << this->content; }

std::string File::getContent() { return this->content; }

void File::setContent(std::string content) { this->content = content; }

void File::clearContents() { this->content.clear(); }
