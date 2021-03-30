cmake_minimum_required(VERSION 3.0)

project(demo_opencv_intrisic_calibration)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(ZKNOTE_DIR ../../../..)

find_package(OpenCV)
message("${OpenCV_INCLUDE_DIRS}")

include_directories(
    ${ZKNOTE_DIR}/3rd-party
    ${OpenCV_INCLUDE_DIRS}
    )

add_executable(
    camera_intrinsic_calibration
    camera_calibration.cpp
    ${BACKWARD_ENABLE}
    )

target_link_libraries(
    camera_intrinsic_calibration
    ${OpenCV_LIBS}
    dw
    )
