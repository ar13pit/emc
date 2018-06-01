cmake_minimum_required(VERSION 2.8)
project(escape_room)

# Include all header files
include_directories(include)

# Search for all source files of extension ".cc" and add them to $SOURCES
set(SOURCES
 src/detection.cpp
 src/driveControl.cpp
 src/main2.cpp
 src/planning.cpp
 src/worldModel.cpp
 src/stateMachine.cpp)

# Create executable named "testPlanning"
add_executable(escape_room ${SOURCES})

# Link libraries
target_link_libraries(escape_room emc-framework)

# Create the executable in directory "/bin"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

