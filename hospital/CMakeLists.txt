cmake_minimum_required(VERSION 2.8)
project(hospital_challenge)

# Include all header files
include_directories(include)

# Search for all source files of extension ".cc" and add them to $SOURCES
set(SOURCES
src/detection.cpp
src/driveControl.cpp
src/monitoring.cpp
src/worldModel.cpp
src/planning.cpp
src/stateMachine.cpp
src/main_explore.cpp)
# Create executable named "testPlanning"
add_executable(hospital_room ${SOURCES})

# Link libraries
target_link_libraries(hospital_room emc-framework)

# Create the executable in directory "/bin"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

