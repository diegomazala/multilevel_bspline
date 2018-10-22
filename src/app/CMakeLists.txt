cmake_minimum_required(VERSION 3.9)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

###########################################################################
#
# bspline_surface_app
#
project(bspline_surface_app)
add_executable(${PROJECT_NAME} bspline_surface_app.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} bspline_surface)

###########################################################################
#
# mba_surface_app
#
project(mba_surface_app)
add_executable(${PROJECT_NAME} mba_surface_app.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} bspline_surface)


###########################################################################
#
# mesh_normalize
#
if (OpenMesh_FOUND)
    project(mesh_normalize VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} mesh_normalize.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} ${OPENMESH_LIBRARIES} mesh_utils)
    else()
        target_link_libraries(${PROJECT_NAME} ${OPENMESH_LIBRARIES} mesh_utils stdc++fs)
    endif()
endif()


###########################################################################
#
# bezier_app
#
if (Eigen3_FOUND)
    project(bezier_app VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} bezier_app.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${OPENMESH_LIBRARIES} bezier)
endif()
    



###########################################################################
#
# bspline_mesh_app
#
if (OpenMesh_FOUND)
	project(bspline_mesh_app VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} bspline_mesh_app.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
	set (LINK_LIBS ${OPENMESH_LIBRARIES} bspline_surface common mesh_utils nanoflann)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS} stdc++fs)
    endif()
endif()




if (OpenMesh_FOUND AND Eigen3_FOUND)

    ###########################################################################
    #
    # bspline_mesh_bezier_app
    #
    project(bspline_mesh_bezier_app VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} bspline_mesh_bezier_app.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
	set (LINK_LIBS ${OPENMESH_LIBRARIES} bspline_surface bezier common mesh_utils nanoflann)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS} stdc++fs)
    endif()

    ###########################################################################
    #
    # bspline_mesh_xyz_app
    #
    project(bspline_mesh_xyz_app VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} bspline_mesh_xyz_app.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
	set (LINK_LIBS ${OPENMESH_LIBRARIES} bspline_surface bezier common mesh_utils nanoflann)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS} stdc++fs)
    endif()
    
endif()




###########################################################################
#
# mba_mesh_app
#
if (OpenMesh_FOUND)
	project(mba_mesh_app VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} mba_mesh_app.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
	set (LINK_LIBS ${OPENMESH_LIBRARIES} bspline_surface common mesh_utils nanoflann)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS} stdc++fs)
    endif()
endif()
