
project( FloodFill )
find_package( OpenCV REQUIRED )
add_executable( FloodFill FloodFill)
target_link_libraries( FloodFill ${OpenCV_LIBS} )
