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


add_executable (environment src/environment.cpp src/render/render.cpp src/processPointClouds.cpp)
target_link_libraries (environment ${PCL_LIBRARIES})

#[[
add_executable (environment_filtered src/environment_filtered.cpp src/render/render.cpp src/processPointClouds.cpp)
target_link_libraries (environment_filtered ${PCL_LIBRARIES})

add_executable (environment_filtered_segamented src/environment_filtered_segamented.cpp src/render/render.cpp src/processPointClouds.cpp)
target_link_libraries (environment_filtered_segamented ${PCL_LIBRARIES})

add_executable (environment_filtered_segamented_clustered src/environment_filtered_segamented_clustered.cpp src/render/render.cpp src/processPointClouds.cpp)
target_link_libraries (environment_filtered_segamented_clustered ${PCL_LIBRARIES})

add_executable (environment_filtered_segamented_clustered_stream src/environment_filtered_segamented_clustered_stream.cpp src/render/render.cpp src/processPointClouds.cpp)
target_link_libraries (environment_filtered_segamented_clustered_stream ${PCL_LIBRARIES})

add_executable (environment_pcl src/environment_pcl.cpp src/render/render.cpp src/processPointClouds.cpp)
target_link_libraries (environment_pcl ${PCL_LIBRARIES})

add_executable (environment_no_pcl src/environment_no_pcl.cpp src/render/render.cpp src/processPointClouds_no_pcl.cpp)
target_link_libraries (environment_no_pcl ${PCL_LIBRARIES})]]
