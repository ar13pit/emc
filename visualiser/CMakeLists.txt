cmake_minimum_required(VERSION 2.8)
project(visualiser3)

# Include all header files
include_directories(include)

# Search for all source files of extension ".cc" and add them to $SOURCES
set(SOURCES
 src/detection.cpp
 src/driveControl.cpp
 src/main2.cpp
 src/planning.cpp
 src/visualize.cpp)
# src/initialization.cpp)

find_package(OpenCV REQUIRED)

# Create executable named "testPlanning"
add_executable(visualiser3 ${SOURCES})

# Link libraries
target_link_libraries(visualiser3 emc-framework)
target_link_libraries(visualiser3 ${OpenCV_LIBS})

# Create the executable in directory "/bin"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

