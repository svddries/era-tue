cmake_minimum_required(VERSION 2.8.3)
project(era)

find_package(era )

include_directories(
    include
    ${era_INCLUDE_DIRS}
)

# ----------------------------------------------------------------------------------------------------

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
set(freenect_LIBRARIES /usr/lib/libfreenect.so)

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
)


add_executable(test_kinect
    test/kinect.cpp
)
target_link_libraries(test_kinect ${era_LIBRARIES} ${freenect_LIBRARIES} ${OpenGL_LIBRARIES} ${GLUT_LIBRARIES})
