
project( Trackbar )
find_package( OpenCV REQUIRED )
add_executable( Trackbar Trackbar )
target_link_libraries( Trackbar ${OpenCV_LIBS} )
