
project( SurfPoint )
find_package( OpenCV REQUIRED )
add_executable(SurfPoint SurfPoint)
target_link_libraries( SurfPoint ${OpenCV_LIBS} )
