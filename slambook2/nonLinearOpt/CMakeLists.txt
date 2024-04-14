# g2o
add_executable(g2oCurveFitting g2oCurveFitting.cpp)
target_link_libraries(g2oCurveFitting ${G2O_LIBRARIES})



# gaussNewton
add_executable(gaussNewton gaussNewton.cpp)
target_link_libraries(gaussNewton ${${OpenCV_LIBS}})