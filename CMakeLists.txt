cmake_minimum_required(VERSION 3.28)
project(goodcal)

set(CMAKE_CXX_STANDARD 20)

# Fix clangd on non-FHS compliant systems
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

find_package(Curses REQUIRED)

file(GLOB_RECURSE goodcal_SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${goodcal_SOURCES})
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
