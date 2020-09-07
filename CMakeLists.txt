cmake_minimum_required(VERSION 3.9)

project(blending_face_details C CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH              ${PROJECT_SOURCE_DIR}/cmake/Modules)

list(APPEND CMAKE_PREFIX_PATH $ENV{EMBREE_DIR})
list(APPEND CMAKE_PREFIX_PATH $ENV{OPENMESH_DIR})

find_package(OpenMesh)
if (NOT OPENMESH_FOUND)
    message(Warning " OpenMesh was not found. Some projects will not be built")
endif()


find_package(Eigen3)
if (NOT Eigen3_FOUND)
    message(Warning " Eigen3 was not found. Some projects will not be built")
endif()

find_package(Threads)
if (NOT Threads_FOUND)
    message(Warning " Threads was not found. Some projects will not be built")
endif()


find_package(embree REQUIRED)
if (NOT Embree_FOUND)
    message(Warning " Embree was not found. Some projects will not be built")
endif()

add_subdirectory(src/lib)
add_subdirectory(src/app)


