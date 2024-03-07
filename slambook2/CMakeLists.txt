cmake_minimum_required(VERSION 3.0.0)
project(slambook2)

include_directories("/usr/include/eigen3")

# ###############
# g2o #
# ###############
set(G2O_BUILD_APPS OFF CACHE BOOL "build g2o apps" FORCE)
set(G2O_BUILD_EXAMPLES OFF CACHE BOOL "build g2o examples" FORCE)
set(BUILD_WITH_MARCH_NATIVE ON CACHE BOOL "build with -march=native" FORCE)
add_subdirectory(3rdparty/g2o)

list(APPEND IncludeList "${PROJECT_BINARY_DIR}/3rdparty/g2o")
list(APPEND IncludeList "3rdparty/g2o")

set(G2O_LIBRARIES
    core
    freeglut_minimal
    opengl_helper
    solver_dense
    solver_eigen
    solver_pcg
    solver_slam2d_linear
    solver_structure_only
    stuff
    types_data
    types_icp
    types_sba
    types_sclam2d
    types_sim3
    types_slam2d_addons
    types_slam2d
    types_slam3d_addons
    types_slam3d
)

message(VERBOSE "G2O_LIBRARIES: " ${G2O_LIBRARIES})

# ###############
# Pangolin #
# ###############

find_package(Sophus REQUIRED)
include_directories(${Sophus_INCLUDE_DIR})

message(VERBOSE "Sophus_INCLUDE_DIR: " ${Sophus_INCLUDE_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

message(WARN "OpenCV_INCLUDE_DIR: " ${OpenCV_INCLUDE_DIRS})

add_subdirectory(useEigen)
add_subdirectory(useGeometry)
add_subdirectory(nonLinearOpt)
add_subdirectory(images)