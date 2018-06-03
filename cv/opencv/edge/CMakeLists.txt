
project( Edge )
find_package( OpenCV REQUIRED )
add_executable( Edge Edge)
target_link_libraries( Edge ${OpenCV_LIBS} )
