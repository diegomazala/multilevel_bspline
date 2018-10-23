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
        ${BSPLINE_SURFACE_INCLUDE_DIR}/matrix.h
)

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
    target_include_directories(${LIB_NAME} INTERFACE ${BEZIER_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR})
    target_sources(${LIB_NAME} INTERFACE ${BEZIER_INCLUDE_DIR}/bezier.h)
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
target_sources(${LIB_NAME} INTERFACE ${COMMON_INCLUDE_DIR}/timer)
