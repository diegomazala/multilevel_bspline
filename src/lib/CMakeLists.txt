cmake_minimum_required(VERSION 3.9)

# Prepare "Catch" library for other executables
set(BSPLINE_SURFACE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bspline_surface)
add_library(BSplineSurface INTERFACE)
target_include_directories(BSplineSurface INTERFACE ${BSPLINE_SURFACE_INCLUDE_DIR})