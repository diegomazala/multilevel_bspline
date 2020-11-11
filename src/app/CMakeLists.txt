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
if (Eigen3_FOUND AND OPEN_MESH_FOUND)
    project(bezier_app VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} bezier_app.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${OPENMESH_LIBRARIES} Eigen3::Eigen bezier)
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
if (OpenMesh_FOUND AND Embree_FOUND)
    project(cdm VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} cdm.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${Embree_INCLUDE_DIRS} ${OPENMESH_INCLUDE_DIRS})
    set (LINK_LIBS ${Embree_LIBRARY} ${OPENMESH_LIBRARIES} common raytracing Threads::Threads)
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
# blending_details_lap
#
if (OpenMesh_FOUND)
    project(blending_details_lap VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} blending_details_lap.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
    set (LINK_LIBS ${OPENMESH_LIBRARIES} common Threads::Threads)
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




###########################################################################
#
# smooth
#
if (OpenMesh_FOUND)
    project(smooth VERSION 0.1 LANGUAGES CXX)
    add_executable(${PROJECT_NAME} smooth.cpp)
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    target_include_directories (${PROJECT_NAME} PRIVATE ${OPENMESH_INCLUDE_DIRS})
    set (LINK_LIBS ${OPENMESH_LIBRARIES} mesh_utils)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
    else()
        target_link_libraries(${PROJECT_NAME} ${LINK_LIBS} stdc++fs)
    endif()
endif()




###########################################################################
#
# obj_validate
#
project(obj_validate)
add_executable(${PROJECT_NAME} obj_validate.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
#target_include_directories (${PROJECT_NAME} PRIVATE ${TINY_OBJ_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} tinyobj)



###########################################################################
#
# obj_extract_group
#
project(obj_extract_group)
add_executable(${PROJECT_NAME} obj_extract_group.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} tinyobj)


###########################################################################
#
# obj_remove_group
#
project(obj_remove_group)
add_executable(${PROJECT_NAME} obj_remove_group.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} tinyobj)



###########################################################################
#
# obj_diff
#
project(obj_diff)
add_executable(${PROJECT_NAME} obj_diff.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} tinyobj Threads::Threads)



###########################################################################
#
# vert_displacement_write
#
project(vert_displacement_write)
add_executable(${PROJECT_NAME} vert_displacement_write.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} common tinyobj Threads::Threads)



###########################################################################
#
# obj_triangulate
#
project(obj_triangulate)
add_executable(${PROJECT_NAME} obj_triangulate.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} tinyobj)


###########################################################################
#
# obj_uv_flip
#
project(obj_uv_flip)
add_executable(${PROJECT_NAME} obj_uv_flip.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} tinyobj)


###########################################################################
# obj_split_file
#
project(obj_split_file)
add_executable(${PROJECT_NAME} obj_split_file.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} tinyobj tinyply common)


###########################################################################
#
# polifit
#
project(polifit)
add_executable(${PROJECT_NAME} polifit.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen common)