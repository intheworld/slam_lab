# openCV
add_executable(cvBasics cvBasics.cpp)
target_link_libraries(cvBasics ${OpenCV_LIBS})

add_executable(cvOrb cvOrb.cpp)
target_link_libraries(cvOrb ${OpenCV_LIBS})


add_executable(pos_estimation_2d2d posEstimation2d2d.cpp)
target_link_libraries(pos_estimation_2d2d ${OpenCV_LIBS})