# Udacity Sensor Fusion Lidar

## Running
mkdir build
cd build
cmake ..
make

### PCL-based
1. Filtering: ./pcl_environment_filtered
2. Segmentation: ./pcl_environment_filtered_segamented
3. Clustering: ./pcl_environment_filtered_segamented_clustered
4. Streaming: ./pcl_environment_filtered_segamented_clustered_stream

### Non-PCL-based
1. Ransac: ./no_pcl_environment_ransac
2. KdTree: ./no_pcl_environment_ransac_kdtree

## Improvement
After applying my own EuclideanCluster following the tutorial, the clustering time increase quite significantly. Need to investigate more to find out the reason.
