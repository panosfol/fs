cmake_minimum_required(VERSION 3.10)

project(filesystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost 1.74.0 REQUIRED COMPONENTS serialization)

include_directories(${BOOST_INCLUDE_DIRS})

file(GLOB SOURCES src/*.cc)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} )
target_include_directories(filesystem PUBLIC ${CMAKE_SOURCE_DIR}/include)
