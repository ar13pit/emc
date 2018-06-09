cmake_minimum_required(VERSION 2.8.3)
project(worldmodel)

add_compile_options(-std=c++11)
#set(HEADERS src/corner.h)

# Include all header files
include_directories(include)

# Create executable named "testPlanning"
add_executable(${PROJECT_NAME} ${SOURCES})

#add_executable(${PROJECT_NAME}_read ${HEADERS} src/read.cpp)
#add_executable(${PROJECT_NAME}_write ${HEADERS} src/write.cpp)


# Link libraries
target_link_libraries(${PROJECT_NAME} emc-framework)

# Create the executable in directory "/bin"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(DATADIR data)
file(MAKE_DIRECTORY ${DATADIR})
