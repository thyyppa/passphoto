cmake_minimum_required( VERSION 3.0 )
project( passphoto VERSION 1.0.1 )
set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -pthread -s" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread -s" )

add_definitions( -DPROJECT_NAME="${PROJECT_NAME}" )
configure_file( src/Version.h.in src/Version.h )

find_package( OpenCV 4.4 REQUIRED COMPONENTS core imgproc imgcodecs objdetect )
find_package( Boost REQUIRED COMPONENTS program_options filesystem )

include_directories( src/ ${PROJECT_BINARY_DIR}/src )

set( SOURCE_FILES main.cpp src/FaceDetector.cpp src/FaceDetector.h src/Config.cpp src/Config.h src/FaceCropper.cpp src/FaceCropper.h src/Bounds.h src/ProgramOutput.cpp src/ProgramOutput.h src/ErrorTypes.h src/Version.h )

add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES} )

install( TARGETS ${PROJECT_NAME} DESTINATION bin )
