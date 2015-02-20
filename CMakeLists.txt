cmake_minimum_required(VERSION 2.8.3)
project(era-tue)

find_package(era )

include_directories(
    include
    ${era_INCLUDE_DIRS}
)


# ----------------------------------------------------------------------------------------------------

add_library(era_tue_config
    include/era-tue/configuration/variant.h
    include/era-tue/configuration/data.h
    include/era-tue/configuration/reader.h    
    include/era-tue/configuration/writer.h    
    include/era-tue/configuration/parser.h
    src/configuration/parser.cpp
    include/era-tue/configuration/emitter.h
    src/configuration/emitter.cpp
)

# ----------------------------------------------------------------------------------------------------

add_library(era_tue_geometry
    include/era-tue/geometry/math.h
    include/era-tue/geometry/mesh.h
    include/era-tue/geometry/rasterizer.h
    src/geometry/rasterizer.cpp
    include/era-tue/geometry/primitives.h
    src/geometry/primitives.cpp
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
#     APPLICATIONS
# ----------------------------------------------------------------------------------------------------

add_executable(wm
    src/applications/wm/main.cpp
    src/applications/wm/localizer.h
    src/applications/wm/localizer.cpp
)
target_link_libraries(wm era_tue_kinect era_tue_config era_tue_geometry ${era_LIBRARIES})

# ----------------------------------------------------------------------------------------------------
#     TESTS
# ----------------------------------------------------------------------------------------------------

add_executable(test_kinect test/test_kinect.cpp)
target_link_libraries(test_kinect era_tue_kinect ${era_LIBRARIES})

add_executable(test_config test/test_config.cpp)
target_link_libraries(test_config era_tue_config ${era_LIBRARIES})

add_executable(test_rasterizer test/test_rasterizer.cpp)
target_link_libraries(test_rasterizer era_tue_geometry ${OpenCV_LIBRARIES})

