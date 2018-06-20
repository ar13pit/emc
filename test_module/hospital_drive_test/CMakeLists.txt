cmake_minimum_required(VERSION 2.8)
project(hospital_challenge)

# Use C++ 11 for JSON library
add_compile_options(-std=c++11)

# Include all header files
include_directories(include)

# Search for all source files of extension ".cc" and add them to $SOURCES
file(GLOB SOURCES "src/*.cpp")

# Create executable named "testPlanning"
add_executable(drive_test ${SOURCES} main/drive_test.cpp)
add_executable(json_test ${SOURCES} main/json_test.cpp)


find_package(OpenCV REQUIRED)

# Link libraries
target_link_libraries(drive_test emc-framework)
target_link_libraries(drive_test ${OpenCV_LIBS})
target_link_libraries(json_test emc-framework)
target_link_libraries(json_test ${OpenCV_LIBS})

# Create the executable in directory "/bin"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
