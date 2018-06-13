cmake_minimum_required(VERSION 2.8)
project(Test)


set(SOURCES
 driveControl.cpp)

add_executable(driveTest ${SOURCES})


target_link_libraries(driveTest emc-framework)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
