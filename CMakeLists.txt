cmake_minimum_required(VERSION 3.9)

project(cpp_starter_project)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH              ${PROJECT_SOURCE_DIR}/cmake/Modules)

add_subdirectory(src/lib)
add_subdirectory(src/app)


