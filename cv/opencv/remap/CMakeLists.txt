
project( Remap )
find_package( OpenCV REQUIRED )
add_executable(Remap Remap)
target_link_libraries( Remap ${OpenCV_LIBS} )
