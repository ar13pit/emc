cmake_minimum_required(VERSION 2.8.9)
project (planning)

# Include all header files
include_directories(include)

# Search for all source files of extension ".cpp" and add them to $SOURCES
file(GLOB SOURCES "src/*.cpp")

# Create executable named "testPlanning"
add_executable(testPlanning ${SOURCES})

# Create the executable in directory "/bin"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
