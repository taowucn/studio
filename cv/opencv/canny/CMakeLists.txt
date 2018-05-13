
project( Canny )
find_package( OpenCV REQUIRED )
add_executable( Canny Canny)
target_link_libraries( Canny ${OpenCV_LIBS} )
