
project( Alpha )
find_package( OpenCV REQUIRED )
add_executable( Alpha Alpha)
target_link_libraries( Alpha ${OpenCV_LIBS} )

project( Mixer )
find_package( OpenCV REQUIRED )
add_executable( Mixer Mixer)
target_link_libraries( Mixer ${OpenCV_LIBS} )

project( Mixer2 )
find_package( OpenCV REQUIRED )
add_executable( Mixer2 Mixer2)
target_link_libraries( Mixer2 ${OpenCV_LIBS} )