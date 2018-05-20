cmake_minimum_required(VERSION 2.8)
project(visualiser)

find_package(OpenCV REQUIRED)
add_executable(viualiser src/visualiser.cpp)
target_link_libraries(visualiser emc-framework)
target_link_libraries(visualiser ${OpenCV_LIBS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
