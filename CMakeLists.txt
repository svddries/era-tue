cmake_minimum_required(VERSION 2.8.3)
project(era-tue)

find_package(era )

include_directories(
    include
    ${era_INCLUDE_DIRS}
)


# ----------------------------------------------------------------------------------------------------

add_library(era_tue_config
    src/configuration/data.cpp
    include/era-tue/configuration/data.h
    src/configuration/reader.cpp
    include/era-tue/configuration/reader.h
    src/configuration/writer.cpp
    include/era-tue/configuration/writer.h
)

# ----------------------------------------------------------------------------------------------------

find_package(OpenCV REQUIRED)
set(freenect_LIBRARIES /usr/lib/libfreenect.so)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
)

add_library(era_tue_kinect
	src/components/kinect.cpp
)
target_link_libraries(era_tue_kinect ${era_LIBRARIES} ${freenect_LIBRARIES} ${OpenCV_LIBRARIES})

# ----------------------------------------------------------------------------------------------------
#     TESTS
# ----------------------------------------------------------------------------------------------------

add_executable(test_kinect test/test_kinect.cpp)
target_link_libraries(test_kinect era_tue_kinect ${era_LIBRARIES})

add_executable(test_config test/test_config.cpp)
