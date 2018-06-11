
project( Hough )
find_package( OpenCV REQUIRED )
add_executable( Hough Hough)
target_link_libraries( Hough ${OpenCV_LIBS} )
