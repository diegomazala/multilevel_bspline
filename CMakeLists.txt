cmake_minimum_required(VERSION 3.9)

project(blending_face_details)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH              ${PROJECT_SOURCE_DIR}/cmake/Modules)


find_package(OpenMesh)
if (NOT OPENMESH_FOUND)
    message(Warning " OpenMesh was not found. Some projects will not be built")
endif()


find_package(Eigen3)
if (NOT Eigen3_FOUND)
    message(Warning " Eigen3 was not found. Some projects will not be built")
endif()



add_subdirectory(src/lib)
add_subdirectory(src/app)


