
project( StereoBM )
find_package( OpenCV REQUIRED )
add_executable( StereoBM SBM_Sample)
target_link_libraries( StereoBM ${OpenCV_LIBS} )
