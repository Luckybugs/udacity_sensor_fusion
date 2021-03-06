// PCL lib Functions for processing point clouds 
#include <unordered_set>
#include "kdtree.h"
#include "processPointClouds.h"


//constructor:
template<typename PointT>
ProcessPointClouds<PointT>::ProcessPointClouds() {}


//de-constructor:
template<typename PointT>
ProcessPointClouds<PointT>::~ProcessPointClouds() {}


template<typename PointT>
void ProcessPointClouds<PointT>::numPoints(typename pcl::PointCloud<PointT>::Ptr cloud)
{
    std::cout << cloud->points.size() << std::endl;
}


template<typename PointT>
typename pcl::PointCloud<PointT>::Ptr ProcessPointClouds<PointT>::FilterCloud(typename pcl::PointCloud<PointT>::Ptr cloud, float filterRes, Eigen::Vector4f minPoint, Eigen::Vector4f maxPoint)
{

    // Time segmentation process
    auto startTime = std::chrono::steady_clock::now();

    // TODO:: Fill in the function to do voxel grid point reduction and region based filtering
    pcl::VoxelGrid<PointT> vg;
    typename pcl::PointCloud<PointT>::Ptr cloudFiltered (new pcl::PointCloud<PointT>);
    vg.setInputCloud(cloud);
    vg.setLeafSize(filterRes, filterRes, filterRes);
    vg.filter(*cloudFiltered);

    typename pcl::PointCloud<PointT>::Ptr cloudRegion (new pcl::PointCloud<PointT>);

    pcl::CropBox<PointT> region(true);
    region.setMin(minPoint);
    region.setMax(maxPoint);
    region.setInputCloud(cloudFiltered);
    region.filter(*cloudRegion);

    std::vector<int> indices;

    pcl::CropBox<PointT> roof(true);
    roof.setMin(Eigen::Vector4f (-1.5, -1.7, -1, 1));
    roof.setMax(Eigen::Vector4f (2.6, 1.7, -.4, 1));
    roof.setInputCloud(cloudRegion);
    roof.filter(indices);

    pcl::PointIndices::Ptr inliers {new pcl::PointIndices};
    for (int point : indices) {
        inliers->indices.push_back(point);
    }

    pcl::ExtractIndices<PointT> extract;
    extract.setInputCloud(cloudRegion);
    extract.setIndices(inliers);
    extract.setNegative(true);
    extract.filter(*cloudRegion);

    auto endTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "filtering took " << elapsedTime.count() << " milliseconds" << std::endl;

    return cloudRegion;

}


template<typename PointT>
std::pair<typename pcl::PointCloud<PointT>::Ptr, typename pcl::PointCloud<PointT>::Ptr> ProcessPointClouds<PointT>::SegmentPlane(typename pcl::PointCloud<PointT>::Ptr cloud, int maxIterations, float distanceThreshold)
{
    // Time segmentation process
    auto startTime = std::chrono::steady_clock::now();

    std::unordered_set<int> inliersResult;
    srand(time(NULL));

    double a, b, c, d;
    auto distance = [&a, &b, &c, &d](double x, double y, double z) {
        return fabs(a*x + b*y + c*z + d) / sqrt(a*a + b*b + c*c);
    };

    // TODO:: Fill in this function to find inliers for the cloud.
    while (maxIterations--) {
        std::unordered_set<int> inliers;
        while (inliers.size() < 3) {
            inliers.insert(rand() % cloud->points.size());
        }

        double x1, y1, z1, x2, y2, z2, x3, y3, z3;

        auto itr = inliers.begin();
        x1 = cloud->points[*itr].x;
        y1 = cloud->points[*itr].y;
        z1 = cloud->points[*itr].z;
        itr++;
        x2 = cloud->points[*itr].x;
        y2 = cloud->points[*itr].y;
        z2 = cloud->points[*itr].z;
        itr++;
        x3 = cloud->points[*itr].x;
        y3 = cloud->points[*itr].y;
        z3 = cloud->points[*itr].z;

        double v3_i, v3_j, v3_k;
        v3_i = (y2 - y1)*(z3 - z1) - (z2 - z1)*(y3 - y1);
        v3_j = (z2 - z1)*(x3 - x1) - (x2 - x1)*(z3 - z1);
        v3_k = (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);

        a = v3_i;
        b = v3_j;
        c = v3_k;
        d = -(v3_i*x1 + v3_j*y1 + v3_k*z1);

        for (size_t i = 0; i < cloud->points.size(); ++i) {
            if (distance(cloud->points[i].x, cloud->points[i].y, cloud->points[i].z) < distanceThreshold) {
                inliers.emplace(i);
            }
        }

        if (inliers.size() > inliersResult.size()) {
            inliersResult = std::move(inliers);
        }
    }

    // Populate the newly allocated point clouds
    typename pcl::PointCloud<PointT>::Ptr  obstCloud(new pcl::PointCloud<PointT>());  // The plane points
    typename pcl::PointCloud<PointT>::Ptr planeCloud(new pcl::PointCloud<PointT>());  // The not-plane points

    for(int index = 0; index < cloud->points.size(); index++)
    {
        PointT point = cloud->points[index];
        if(inliersResult.count(index))
            planeCloud->points.push_back(point);
        else
            obstCloud->points.push_back(point);
    }

    auto endTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "plane segmentation took " << elapsedTime.count() << " milliseconds" << std::endl;

    return std::pair<typename pcl::PointCloud<PointT>::Ptr, typename pcl::PointCloud<PointT>::Ptr> (obstCloud, planeCloud);
}


static void clusterHelper(int index, const std::vector<std::vector<float>> points, std::vector<int>& cluster, std::vector<bool>& processed, KdTree* tree, float distanceTol) {
    processed[index] = true;
    cluster.push_back(index);
    std::vector<int> nearest = tree->search(points[index], distanceTol);

    for (int idx : nearest) {
        if (!processed[idx]) {
            clusterHelper(idx, points, cluster, processed, tree, distanceTol);
        }
    }
}

static std::vector<std::vector<int>> euclideanCluster(const std::vector<std::vector<float>>& points, KdTree* tree, float distanceTol, int minSize, int maxSize)
{
    std::vector<std::vector<int>> clusters;
    std::vector<bool> processed(points.size(), false);

    int i = 0;

    while (i < points.size()) {
        if (processed[i]) {
            i++;
            continue;
        }

        std::vector<int> cluster;
        clusterHelper(i, points, cluster, processed, tree, distanceTol);

        clusters.push_back(cluster);

        i++;
    }

    return clusters;
}

template<typename PointT>
std::vector<typename pcl::PointCloud<PointT>::Ptr> ProcessPointClouds<PointT>::Clustering(typename pcl::PointCloud<PointT>::Ptr cloud, float clusterTolerance, int minSize, int maxSize)
{
    // Time the clustering step
    auto startTime = std::chrono::steady_clock::now();

    std::vector<typename pcl::PointCloud<PointT>::Ptr> clusters;
    std::vector<std::vector<float>> pts;

    KdTree* tree = new KdTree;
    for (int i = 0; i < cloud->points.size(); i++) {
        auto pt = cloud->points[i];
        pts.push_back(std::vector<float> {pt.x, pt.y, pt.z});
        tree->insert(std::vector<float> {pt.x, pt.y, pt.z}, i);
    }

    std::vector<std::vector<int>> clusterIndices = euclideanCluster(pts, tree, clusterTolerance, minSize, maxSize);

    for (auto clusterIndex : clusterIndices) {
        typename pcl::PointCloud<PointT>::Ptr cloudCluster (new pcl::PointCloud<PointT>);
        for (auto pointIndex : clusterIndex) {
            cloudCluster->points.push_back (cloud->points[pointIndex]);
        }
        cloudCluster->width = cloudCluster->points.size();
        cloudCluster->height = 1;
        cloudCluster->is_dense = true;

        clusters.push_back(cloudCluster);
    }

    auto endTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "clustering took " << elapsedTime.count() << " milliseconds and found " << clusters.size() << " clusters" << std::endl;

    return clusters;
}


template<typename PointT>
Box ProcessPointClouds<PointT>::BoundingBox(typename pcl::PointCloud<PointT>::Ptr cluster)
{

    // Find bounding box for one of the clusters
    PointT minPoint, maxPoint;
    pcl::getMinMax3D(*cluster, minPoint, maxPoint);

    Box box;
    box.x_min = minPoint.x;
    box.y_min = minPoint.y;
    box.z_min = minPoint.z;
    box.x_max = maxPoint.x;
    box.y_max = maxPoint.y;
    box.z_max = maxPoint.z;

    return box;
}


template<typename PointT>
void ProcessPointClouds<PointT>::savePcd(typename pcl::PointCloud<PointT>::Ptr cloud, std::string file)
{
    pcl::io::savePCDFileASCII (file, *cloud);
    std::cerr << "Saved " << cloud->points.size () << " data points to "+file << std::endl;
}


template<typename PointT>
typename pcl::PointCloud<PointT>::Ptr ProcessPointClouds<PointT>::loadPcd(std::string file)
{

    typename pcl::PointCloud<PointT>::Ptr cloud (new pcl::PointCloud<PointT>);

    if (pcl::io::loadPCDFile<PointT> (file, *cloud) == -1) //* load the file
    {
        PCL_ERROR ("Couldn't read file \n");
    }
    std::cerr << "Loaded " << cloud->points.size () << " data points from "+file << std::endl;

    return cloud;
}


template<typename PointT>
std::vector<boost::filesystem::path> ProcessPointClouds<PointT>::streamPcd(std::string dataPath)
{

    std::vector<boost::filesystem::path> paths(boost::filesystem::directory_iterator{dataPath}, boost::filesystem::directory_iterator{});

    // sort files in accending order so playback is chronological
    sort(paths.begin(), paths.end());

    return paths;

}