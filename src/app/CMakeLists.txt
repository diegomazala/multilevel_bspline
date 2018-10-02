cmake_minimum_required(VERSION 3.9)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

project(bspline_surface_app)
add_executable(${PROJECT_NAME} bspline_surface_app.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} BSplineSurface)


project(mba_surface_app)
add_executable(${PROJECT_NAME} mba_surface_app.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} BSplineSurface)
