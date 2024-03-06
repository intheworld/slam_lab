add_executable(useGeometry useGeometry.cpp)
add_executable(coordinateTransform coordinateTransform.cpp)
add_executable(useSophus useSophus.cpp)

target_link_libraries(useSophus Sophus::Sophus)
