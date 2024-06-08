# openCV
add_executable(cvBasics cvBasics.cpp)
target_link_libraries(cvBasics ${OpenCV_LIBS})

add_executable(cvOrb cvOrb.cpp)
target_link_libraries(cvOrb ${OpenCV_LIBS})