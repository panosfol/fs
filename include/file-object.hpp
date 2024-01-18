#ifndef OBJECT_H_NAME
#define OBJECT_H_NAME

#include <boost/serialization/nvp.hpp>
#include <iostream>

#include <boost/serialization/version.hpp>
#if BOOST_VERSION / 100000 == 1 && BOOST_VERSION / 100 % 1000 == 74
#include <boost/serialization/library_version_type.hpp>
#endif

#define ROOT "FileSystem Root Directory"

// Forward declaration of Directory class to be used in the FileObject class
class Directory;

// The different type of files that are supported
enum filetype { FSFILE, FSDIRECTORY };

class FileObject {
private:
        friend class boost::serialization::access;
        template <class Ar> void serialize(Ar &ar, unsigned) {
                ar &BOOST_NVP(name) & BOOST_NVP(date_of_creation) &
                    BOOST_NVP(type) & BOOST_NVP(parent_dir) &
                    BOOST_NVP(absolute_path);
        }

protected:
        std::string name;
        std::string date_of_creation;
        filetype type;
        Directory *parent_dir;
        std::string absolute_path;

public:
        FileObject() = default; // for deserialization
        FileObject(std::string, filetype, Directory *);

        virtual ~FileObject() = default;

        std::string getName() const;
        void setName(std::string);

        void setAbsolutePath(std::string, std::string);
        void setAbsolutePath(std::string);
        std::string getAbsolutePath();

        void setParentDir(Directory *);
        Directory *getParentDir();

        std::string getDate();

        filetype getType();
};
#endif
