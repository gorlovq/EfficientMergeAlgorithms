cmake_minimum_required(VERSION 3.27) 
project(merge_algs CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})