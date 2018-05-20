cmake_minimum_required(VERSION 2.8)
project(example)


add_executable(main src/main.cpp)


target_link_libraries(main emc-framework)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
