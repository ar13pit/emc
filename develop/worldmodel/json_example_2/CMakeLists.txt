cmake_minimum_required(VERSION 2.8.3)
project(json_example)

add_compile_options(-std=c++11)
set(HEADERS src/corner.h)

add_executable(${PROJECT_NAME}_read ${HEADERS} src/read.cpp)
add_executable(${PROJECT_NAME}_write ${HEADERS} src/write.cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
