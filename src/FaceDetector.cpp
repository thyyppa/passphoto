#include "FaceDetector.h"

FaceDetector::FaceDetector( Config &config, ProgramOutput &output ) : config( config ), output( output )
{
    ensureCascadeFileExists();
    ensureInputFileExists();
}

/**
 * This is the brains of the operation. It uses OpenCV to detect faces in the input file and
 * then adds the face boundaries (Bounds) to the `faces` property (FacesDetected vector)
 *
 * This method does not do any image manipulation. See FaceCropper.cpp for that.
 *
 * @return FacesDetected
 */
FacesDetected FaceDetector::detectFaces()
{
    try
    {
        boost::filesystem::path path( config.getInputPath());

        // read in input image
        cv::Mat image = imread( config.getInputPath(), cv::IMREAD_COLOR );

        // set a minimum face size so we don't detect faces in clothing and such
        cv::Size min_size = {
                (int) ( image.size[ 0 ] * 0.1 ),
                (int) ( image.size[ 1 ] * 0.1 )
        };

        // set up cascade classifier
        cv::CascadeClassifier cascade;
        cascade.load( config.getCascadePath());

        // detect faces
        std::vector<cv::Rect> faces;
        cascade.detectMultiScale( image, faces, 1.1, 2, cv::CASCADE_SCALE_IMAGE, min_size );

        // loop through faces and push them to array for output
        for ( auto &face : faces )
        {
            this->faces.push_back(
                    Bounds{
                            face.x, face.y, face.width, face.height
                    }
            );
        }

    } catch ( cv::Exception e )
    {
        output.errorAndExit( CV_ERROR, e.msg.c_str());
    }

    return faces;
}

/**
 * Exits program with an error if cascade file doesn't exist
 */
void FaceDetector::ensureCascadeFileExists()
{
    if ( config.getCascadePath().empty())
        return;

    fs::path path( config.getCascadePath());

    if ( fs::exists( path ))
    {
        if ( fs::is_regular_file( path ))
            return;

        if ( fs::is_directory( path ))
            output.errorAndExit(
                    CASCADE_ERROR,
                    "The cascade path: \'" + path.string() + "\' is a directory."
            );
    }

    output.errorAndExit( CASCADE_ERROR, "Could not find cascade file at: \'" + path.string() + "\'." );
}


/**
 * Exits program with an error if input file doesn't exist
 */
void FaceDetector::ensureInputFileExists()
{
    fs::path path( config.getInputPath());

    if ( fs::exists( path ))
    {
        if ( fs::is_regular_file( path ))
            return;

        if ( fs::is_directory( path ))
            output.errorAndExit( FILE_ERROR, "The input file: \'" + path.string() + "\' is a directory." );
    }

    output.errorAndExit( FILE_ERROR, "Could not find input file at: \'" + path.string() + "\'" );
}
