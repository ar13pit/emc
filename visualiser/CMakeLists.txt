cmake_minimum_required(VERSION 2.8)
project(myperception)

find_package(OpenCV REQUIRED)
add_executable(myperception src/myperception.cpp)

target_link_libraries(myperception emc-framework)
target_link_libraries(myperception ${OpenCV_LIBS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
