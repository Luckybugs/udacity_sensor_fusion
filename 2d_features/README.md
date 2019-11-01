# Udacity Sensor Fusion Camera

## Dependencies
1. OpenCV4.1: sh installOpenCV-4-on-Ubuntu-16-04.sh 

## Running
1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./2D_feature_tracking

## Tasks
### MP.1 Data buffer optimization
In MidTermProject_Camera_Student.cpp, use **std::deque** and set **dataBufferSize = 2**, then use **.pop_font()** to drop the previous item. 

### MP.2 Keypoint detection
In MidTermProject_Camera_Student.cpp, replace **detectorType** with different detectors.

### MP.3 Keypoint removal
In MidTermProject_Camera_Student.cpp, use **vehicleRect.contains(kp.pt)** to keep keypoints within defined **vehicleRect**.

### MP.4 Keypoint descriptors
In MidTermProject_Camera_Student.cpp, replace **descriptorType** with different descriptors.

### MP.5 Descriptor matching
In matching2D_Student.cpp,  
- `descriptorType`: either: `DES_BINARY`, `DES_HOG`
- `matcherType`: either: `MAT_FLANN`, `MAT_BF`
- `selectorType`: either: `SEL_NN`, `SEL_KNN`

### MP.6 Descriptor distance ratio
In matching2D_Student.cpp, set **minDescDistRatio = 0.8**.

### MP.7 Performance keypoints evaluation
As results/mp7.csv shows, FAST produces the most keypoints (about 400 per image) to be the clear winner. 

### MP.8 Performance matching keypoints evaluation
As results/mp8_mp9.csv shows, FAST detector with BRIEF, SIFT, and ORB descriptors produce the most matched keypoints (about 300 per image) to be among the best.

### MP.9 Performance recommendation
As results/mp8_mp9.csv shows, FAST detector with BRIEF, SIFT, and ORB are recommended. However, SIFT is quite tricky to install on Linux. That's why I attached a installation script. 


