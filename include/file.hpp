#ifndef FILE_H_NAME
#define FILE_H_NAME

#include "file-object.hpp"

// Even latest Debian Sid (March 2023) uses Boost 1.74 which does not behave
// well with very fresh compilers and triggers a compiler error. This bug was
// fixed in fresh Boost versions:
// https://github.com/boostorg/serialization/issues/219 and the following 4
// lines act as a workournd only for 1.74

#include <boost/serialization/version.hpp>
#if BOOST_VERSION / 100000 == 1 && BOOST_VERSION / 100 % 1000 == 74
#include <boost/serialization/library_version_type.hpp>
#endif

#include <boost/serialization/list.hpp>

class File : public FileObject {
private:
        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive &ar, const unsigned int) {
                ar &BOOST_SERIALIZATION_BASE_OBJECT_NVP(FileObject);
                ar &BOOST_NVP(content);
        }

protected:
        std::string content;

public:
        File(){};
        File(std::string, filetype file_type, Directory *parent_dir);

	void writeToFile();

	void setContent(std::string);
	std::string getContent();
	void printContent();
	void clearContents();
};
#endif
