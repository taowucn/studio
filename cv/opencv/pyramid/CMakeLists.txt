
project( Pyramid )
find_package( OpenCV REQUIRED )
add_executable( Pyramid Pyramid)
target_link_libraries( Pyramid ${OpenCV_LIBS} )
