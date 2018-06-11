
project( Harris )
find_package( OpenCV REQUIRED )
add_executable( Harris Harris)
target_link_libraries( Harris ${OpenCV_LIBS} )
