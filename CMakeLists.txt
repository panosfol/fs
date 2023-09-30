cmake_minimum_required(VERSION 3.10)

project(filesystem)

file(GLOB SOURCES src/*.cc)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(filesystem PUBLIC ${CMAKE_SOURCE_DIR}/include)
