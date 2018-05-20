
project( Morph )
find_package( OpenCV REQUIRED )
add_executable( Morph Morph)
target_link_libraries( Morph ${OpenCV_LIBS} )


project( MorphologyEx )
find_package( OpenCV REQUIRED )
add_executable( MorphologyEx MorphologyEx)
target_link_libraries( MorphologyEx ${OpenCV_LIBS} )