
project( SurfPoint )
find_package( OpenCV REQUIRED )
add_executable(SurfPoint SurfPoint)
target_link_libraries( SurfPoint ${OpenCV_LIBS} )



project( SurfPoint2 )
find_package( OpenCV REQUIRED )
add_executable(SurfPoint2 SurfPoint2)
target_link_libraries( SurfPoint2 ${OpenCV_LIBS} )
