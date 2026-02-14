cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME Flock)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

find_package(SFML CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
