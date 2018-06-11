cmake_minimum_required(VERSION 2.8)
project(detection_3)


add_executable(main src/main.cpp src/detection.cpp src/visualize.cpp)

find_package(OpenCV REQUIRED)

target_link_libraries(main emc-framework)

target_link_libraries(main ${OpenCV_LIBS})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
