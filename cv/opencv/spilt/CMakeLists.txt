
project( Split )
find_package( OpenCV REQUIRED )
add_executable( Split Split)
target_link_libraries( Split ${OpenCV_LIBS} )
