cmake_minimum_required(VERSION 2.8)
project(detection)



# Include all header files
include_directories(include)


# Search for all source files of extension ".cc" and add them to $SOURCES
set(SOURCES
 src/detection.cpp
# src/driveControl.cpp
 src/main.cpp
# src/planning.cpp
 src/visualize.cpp)
# src/initialization.cpp)

find_package(OpenCV REQUIRED)

# Create executable named "testPlanning"
add_executable(main ${SOURCES})


# Link libraries
target_link_libraries(main emc-framework)
target_link_libraries(main ${OpenCV_LIBS})

# Create the executable in directory "/bin"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)










