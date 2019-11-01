# Udacity Sensor Fusion Camera

## Dependencies
1. OpenCV 4.1
    git clone https://github.com/opencv/opencv.git
    git clone https://github.com/opencv/opencv_contrib.git
    cd opencv & mkdir build & cd build
    cmake -D CMAKE_INSTALL_PREFIX=/usr/local \
            -D OPENCV_ENABLE_NONFREE=ON \
            -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
    make -j4
    make install

## Running
1. mkdir build
2. cd build
3. cmake ..
4. make