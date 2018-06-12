cmake_minimum_required(VERSION 2.8.3)
project(worldmodel)

add_compile_options(-std=c++11)

# Include all header files
include_directories(include)

# Set path to directory "data"
set(DATADIR data)

# Create DATADIR
file(MAKE_DIRECTORY ${DATADIR})

# Add all sources
file(GLOB SOURCES "src/*.cpp")

# Create executable named "testPlanning"
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
#target_link_libraries(${PROJECT_NAME} emc-framework)

# Create the executable in directory "/bin"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

