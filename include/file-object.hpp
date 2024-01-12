#ifndef OBJECT_H_NAME
#define OBJECT_H_NAME

#include <boost/serialization/nvp.hpp>
#include <iostream>

#include <boost/serialization/version.hpp>
#if BOOST_VERSION / 100000 == 1 && BOOST_VERSION / 100 % 1000 == 74
#include <boost/serialization/library_version_type.hpp>
#endif

// The different type of files that are supported
enum filetype { FSFILE, FSDIRECTORY };

class FileObject {
private:
        friend class boost::serialization::access;
        template <class Ar> void serialize(Ar &ar, unsigned) {
                ar &BOOST_NVP(name) & BOOST_NVP(date_of_creation) &
                    BOOST_NVP(type);
        }

protected:
        std::string name;
        std::string date_of_creation;
        filetype type;

public:
        FileObject() = default; // for deserialization
        FileObject(std::string, filetype);

        virtual ~FileObject() = default;

        std::string getName() const;
        void setName(std::string);

        std::string getDate();

        filetype getType();
};
#endif
