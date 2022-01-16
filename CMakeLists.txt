cmake_minimum_required(VERSION 3.17)
project(BTree)

include_directories(src)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES src/node.h src/btree.h)

add_executable(main main.cpp ${SOURCE_FILES})