cmake_minimum_required(VERSION 2.8.3)
project(era)

find_package(era )

include_directories(
    include
    ${era_INCLUDE_DIRS}
)

# ----------------------------------------------------------------------------------------------------

find_package(OpenCV REQUIRED)
set(freenect_LIBRARIES /usr/lib/libfreenect.so)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
)

message( "OpenCV libraries: ${OpenCV_LIBRARIES}" )

add_executable(test_kinect
    test/kinect.cpp
)
target_link_libraries(test_kinect ${era_LIBRARIES} ${freenect_LIBRARIES} ${OpenCV_LIBRARIES})
