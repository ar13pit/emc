cmake_minimum_required(VERSION 2.8)
project(hospital_challenge)

# Use C++ 11 for JSON library
add_compile_options(-std=c++11)

# Include all header files
include_directories(include)

# Search for all source files of extension ".cc" and add them to $SOURCES
file(GLOB SOURCES "src/*.cpp")

# Create executable named "testPlanning"
add_executable(hospital_room ${SOURCES})

find_package(OpenCV REQUIRED)

# Link libraries
target_link_libraries(hospital_room emc-framework)
target_link_libraries(hospital_room ${OpenCV_LIBS})

# Create the executable in directory "/bin"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

