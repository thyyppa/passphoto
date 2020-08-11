#include <Config.h>
#include <FaceDetector.h>
#include <FaceCropper.h>

#include <iostream>
#include <string>

int main( int argc, char *argv[] )
{
    ProgramOutput output;

    try
    {
        Config config( argc, argv, output );

        FaceDetector detector( config, output );
        FaceCropper  cropper( config, output );

        FacesDetected faces = detector.detectFaces();
        cropper.cropFaces( faces );

        std::cout << output.toString();

        return 0;
    } catch ( const std::exception &e )
    {
        output.errorAndExit( GENERAL_ERROR, e.what());
    }

    return -1;
}
