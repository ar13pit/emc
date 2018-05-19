cmake_minimum_required(VERSION 2.8)
project(example)


set(SOURCES
 src/detection.cpp
 src/driveControl.cpp
 src/worldModel.cpp
 src/main.cpp
 src/planning.cpp
 src/helper.hpp)

add_executable(main ${SOURCES})


target_link_libraries(main emc-framework)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
