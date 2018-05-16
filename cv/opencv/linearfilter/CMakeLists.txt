
project( Filters )
find_package( OpenCV REQUIRED )
add_executable( Filters Filters)
target_link_libraries( Filters ${OpenCV_LIBS} )
