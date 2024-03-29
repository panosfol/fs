#ifndef DIR_H_NAME
#define DIR_H_NAME

#include <boost/serialization/unique_ptr.hpp>

#include "file-object.hpp"
#include "file.hpp"

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
#include <boost/serialization/unordered_map.hpp>

class Directory : public FileObject {
private:
        friend class boost::serialization::access;

        template <class Ar> void serialize(Ar &ar, unsigned) {
                ar &BOOST_SERIALIZATION_BASE_OBJECT_NVP(FileObject) &
                    BOOST_NVP(contents);
        }

        std::unordered_map<std::string, std::unique_ptr<FileObject>> contents;

public:
        Directory() = default;            // for deserialization
        Directory(std::string, filetype); // used for creating he root dir only
        Directory(std::string, filetype, Directory *);

        FileObject *findOneContent(std::string);
        std::unordered_map<std::string, std::unique_ptr<FileObject>> &
        getContents();
        void insertContent(std::unique_ptr<FileObject>);
        void removeContent(std::string);

        void moveContent(Directory *, std::string);
        void moveDirectory(Directory *, std::string);
        void moveFile(Directory *, std::string);

        void copyContent(Directory *, std::string);

        void listContents();

        int checkObjName(std::string);
        /*
         * This method changes all the absolute paths of the directories
         * inside a directory. It works recursively.
         */
        void changeAbsPathOfContentDirs();
};
#endif
