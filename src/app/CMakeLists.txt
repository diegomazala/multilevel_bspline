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


###########################################################################
#
# mba_mesh_z_app
#
if (OpenMesh_FOUND)
	project(mba_mesh_z_app VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} mba_mesh_z_app.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
	set (LINK_LIBS ${OPENMESH_LIBRARIES} bspline_surface common mesh_utils nanoflann)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS} stdc++fs)
    endif()
endif()




###########################################################################
#
# cdm
#
if (OpenMesh_FOUND)
    project(cdm VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} cdm.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${EMBREE_INCLUDE_DIRS} ${OPENMESH_INCLUDE_DIRS})
    set (LINK_LIBS ${EMBREE_LIBRARY} ${OPENMESH_LIBRARIES} common raytracing Threads::Threads)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS} stdc++fs)
    endif()
endif()

###########################################################################
#
# blending_details_cdm
#
if (OpenMesh_FOUND)
    project(blending_details_cdm VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} blending_details_cdm.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
    set (LINK_LIBS ${OPENMESH_LIBRARIES} common mesh_utils Threads::Threads)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS} stdc++fs)
    endif()
endif()



###########################################################################
#
# mesh_cut
#
if (OpenMesh_FOUND)
    project(mesh_cut VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} mesh_cut.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
    set (LINK_LIBS ${OPENMESH_LIBRARIES} mesh_utils)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS} stdc++fs)
    endif()
endif()
