cmake_minimum_required(VERSION 3.9)

###########################################################################
#
# bspline_surface lib
#
set(BSPLINE_SURFACE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bspline_surface)
set(LIB_NAME bspline_surface)
add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE ${BSPLINE_SURFACE_INCLUDE_DIR})
target_sources(${LIB_NAME} INTERFACE
        ${BSPLINE_SURFACE_INCLUDE_DIR}/bspline_surface.h
        ${BSPLINE_SURFACE_INCLUDE_DIR}/mba_surface.h
        ${BSPLINE_SURFACE_INCLUDE_DIR}/matrix.h)

###########################################################################
#
# mesh_utils lib
#
if (OpenMesh_FOUND)
    set(MESH_UTILS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mesh_utils)
    set(LIB_NAME mesh_utils)
	add_library(${LIB_NAME} INTERFACE)
    target_include_directories(${LIB_NAME} INTERFACE ${MESH_UTILS_INCLUDE_DIR})
    target_sources(${LIB_NAME} INTERFACE ${MESH_UTILS_INCLUDE_DIR}/mesh_utils.h)
endif()


###########################################################################
#
# bezier lib
#
if (Eigen3_FOUND)
    set(BEZIER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bezier)
	set(LIB_NAME bezier)
	add_library(${LIB_NAME} INTERFACE)
    target_include_directories(${LIB_NAME} INTERFACE ${BEZIER_INCLUDE_DIR})
    target_sources(${LIB_NAME} INTERFACE ${BEZIER_INCLUDE_DIR}/bezier.h Eigen3::Eigen)
endif()


###########################################################################
#
# nanoflann lib
#
set(NANOFLANN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nanoflann)
set(LIB_NAME nanoflann)
add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE ${NANOFLANN_INCLUDE_DIR})
target_sources(${LIB_NAME} INTERFACE 
    ${NANOFLANN_INCLUDE_DIR}/nanoflann.hpp 
    ${NANOFLANN_INCLUDE_DIR}/nanoflann_pointcloud.hpp)



###########################################################################
#
# common lib
#
set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)
set(LIB_NAME common)
add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE ${COMMON_INCLUDE_DIR})
target_sources(${LIB_NAME} INTERFACE 
    ${COMMON_INCLUDE_DIR}/timer.h
    ${COMMON_INCLUDE_DIR}/cmdline.h)



###########################################################################
#
# raytracing lib
#
set(RAYTRACING_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/raytracing)
set(LIB_NAME raytracing)
add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE ${RAYTRACING_INCLUDE_DIR})
target_sources(${LIB_NAME} INTERFACE 
    ${RAYTRACING_INCLUDE_DIR}/geometry.h
    ${RAYTRACING_INCLUDE_DIR}/raytri.h)


###########################################################################
#
# stb_image lib
#
set(STB_IMAGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
set(LIB_NAME stb_image)
add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE ${STB_IMAGE_INCLUDE_DIR})
target_sources(${LIB_NAME} INTERFACE 
    ${STB_IMAGE_INCLUDE_DIR}/stb_image.h
    ${STB_IMAGE_INCLUDE_DIR}/stb_image_write.h
    ${STB_IMAGE_INCLUDE_DIR}/stb_image_resize.h)



###########################################################################
#
# tinyobj lib
#
set(TINY_OBJ_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinyobj)
set(LIB_NAME tinyobj)
add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE ${TINY_OBJ_INCLUDE_DIR})
target_sources(${LIB_NAME} INTERFACE 
    ${TINY_OBJ_INCLUDE_DIR}/tiny_obj_loader.h
    #${TINY_OBJ_INCLUDE_DIR}/tiny_obj_loader.cc
	${TINY_OBJ_INCLUDE_DIR}/tinyobj.h)


###########################################################################
#
# tinyply lib
#
set(TINY_PLY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinyply)
set(LIB_NAME tinyply)
add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE ${TINY_PLY_INCLUDE_DIR})
target_sources(${LIB_NAME} INTERFACE 
    ${TINY_PLY_INCLUDE_DIR}/tinyply.h
    ${TINY_PLY_INCLUDE_DIR}/tinyply.cpp)



###########################################################################
#
# yaml lib
#
set(YAML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/yaml)
set(LIB_NAME yaml)
add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE ${YAML_INCLUDE_DIR})
target_sources(${LIB_NAME} INTERFACE 
    ${YAML_INCLUDE_DIR}/Yaml.hpp
    ${YAML_INCLUDE_DIR}/Yaml.cpp)    