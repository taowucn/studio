
project( Surf )
find_package( OpenCV REQUIRED )
add_executable(Surf Surf)
target_link_libraries( Surf ${OpenCV_LIBS} )
