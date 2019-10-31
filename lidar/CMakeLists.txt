cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_definitions(-std=c++11)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(playback)

find_package(PCL 1.2 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")


add_executable (environment src/environment.cpp src/render/render.cpp src/no_pcl_processPointClouds_ransac_kdtree.cpp)
target_link_libraries (environment ${PCL_LIBRARIES})

add_executable (pcl_environment_filtered src/pcl_environment_filtered.cpp src/render/render.cpp src/processPointClouds.cpp)
target_link_libraries (pcl_environment_filtered ${PCL_LIBRARIES})

add_executable (pcl_environment_filtered_segamented src/pcl_environment_filtered_segamented.cpp src/render/render.cpp src/processPointClouds.cpp)
target_link_libraries (pcl_environment_filtered_segamented ${PCL_LIBRARIES})

add_executable (pcl_environment_filtered_segamented_clustered src/pcl_environment_filtered_segamented_clustered.cpp src/render/render.cpp src/processPointClouds.cpp)
target_link_libraries (pcl_environment_filtered_segamented_clustered ${PCL_LIBRARIES})

add_executable (pcl_environment_filtered_segamented_clustered_stream src/pcl_environment_filtered_segamented_clustered_stream.cpp src/render/render.cpp src/processPointClouds.cpp)
target_link_libraries (pcl_environment_filtered_segamented_clustered_stream ${PCL_LIBRARIES})

add_executable (no_pcl_environment_ransac src/no_pcl_environment_ransac.cpp src/render/render.cpp src/no_pcl_processPointClouds_ransac.cpp)
target_link_libraries (no_pcl_environment_ransac ${PCL_LIBRARIES})

add_executable (no_pcl_environment_ransac_kdtree src/no_pcl_environment_ransac_kdtree.cpp src/render/render.cpp src/no_pcl_processPointClouds_ransac_kdtree.cpp)
target_link_libraries (no_pcl_environment_ransac_kdtree ${PCL_LIBRARIES})