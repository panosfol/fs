cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(filesystem)

file(GLOB SOURCES src/*.cc)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(filesystem PUBLIC ${CMAKE_SOURCE_DIR}/include)
