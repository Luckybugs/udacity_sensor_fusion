# Udacity Sensor Fusion Camera

## Dependencies
1. OpenCV4.1: sh installOpenCV-4-on-Ubuntu-16-04.sh
2. YoloV3 weights: !wget "https://pjreddie.com/media/files/yolov3.weights" if following error message shows:

"terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(4.1.2-dev) darknet/darknet_io.cpp:711: error: (-213:The function/feature is not implemented) Transpose the weights (except for convolutional) is not implemented in function 'ReadDarknetFromWeightsStream'
Aborted (core dumped)"


## Running
1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./3D_object_tracking

## Tasks
### FP.1 Match 3D objects
In camFusion_Student.cpp, **matchBoundingBoxes** uses **std::multimap<int,int>** to track pairs of bounding box IDs that are the best matches between frames.

### FP.2 Compute lidar-based TTC
In camFusion_Student.cpp, first **sortLidarPointsX** sort the vector of lidar points, then **computeTTCLidar** uses **median x-distance** to avoid outliers, then uses **constant velocity model** calculated the TTC.

### FP.3 Associate keypoint matches with bounding boxes
In camFusion_Student.cpp, **clusterKptMatchesWithROI** loops through every matched keypoint pair in an image. If the keypoint falls within the bounding box (ROI) in the current frame, the keypoint match is associated with the current BoundingBox.

### FP.4 Compute mono camera-based TTC
In camFusion_Student.cpp, **computeTTCCamera** uses distance ratios on keypoints matched between frames to determine the rate of scale change within an image. This rate of scale change can be used to estimate the TTC.

TTC = (-1.0 / frameRate) / (1 - medianDistRatio);

Like the lidar TTC estimation, this function uses the median distance ratio to avoid outliers. 

### FP.5 Performance evaluation, lidar outliers
Taking the median point has avoided the outliers. 

### FP.6 Performance evaluation, detector/descriptor combinations
As results/fp6.csv shows, FAST detector with BRIEF, SIFT, and AKAZE are recommended. SIFT is quite tricky to install on Linux. again I attached a installation script.
