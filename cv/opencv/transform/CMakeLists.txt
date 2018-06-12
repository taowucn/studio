
project( Affine )
find_package( OpenCV REQUIRED )
add_executable(Affine Affine)
target_link_libraries( Affine ${OpenCV_LIBS} )
