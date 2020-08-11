#include "FaceCropper.h"

FaceCropper::FaceCropper( Config &config, ProgramOutput &output ) : config( config ), output( output )
{
    ensureInputFileExists();
    ensureOutputPathExists();
}

int FaceCropper::cropFaces( FacesDetected &faces )
{
    int         count = 0;
    std::string output_filename, debug_filename;

    try
    {
        boost::filesystem::path path( config.getInputPath());

        cv::Mat image, output_mat, debug_mat;

        // read input image
        image = imread( config.getInputPath(), cv::IMREAD_COLOR );

        // read image into debug mat if requested
        if ( config.isDebug())
            debug_mat = imread( config.getInputPath(), cv::IMREAD_COLOR );

        // sort faces from largest to smallest
        std::sort(faces.begin(),faces.end(),std::greater<Bounds>());

        // loop through faces
        for ( Bounds &face : faces )
        {
            output_filename = generateFilename( path, count );

            // calculate crop bounds
            Bounds crop = { 0, 0, face.width, face.height };
            crop = matchCropToOutputAspectRatio( crop, face );
            crop = centerCropOnFace( crop, face );
            crop = addPaddingToCrop( crop );

            // output debug images if requested
            if ( config.isDebug())
            {
                debug_filename = generateFilename( path, -1, "debug" );

                rectangle( debug_mat, cv::Rect( crop.x, crop.y, crop.width, crop.height ), { 0, 255, 0 }, 8 );
                rectangle( debug_mat, cv::Rect( face.x, face.y, face.width, face.height ), { 0, 0, 255 }, 8 );

                cv::imwrite( debug_filename, debug_mat );

                output.setDebugFile( debug_filename );
            }

            preventCroppingOutsideImageBounds( crop, image );

            // add bounds for stdio/json output
            output.addOutputFaceBounds(
                    FaceCropBounds{ face, crop }
            );

            // save cropped image if not in detect-only mode
            if ( config.getMode() != DETECT_ONLY )
            {
                output_mat = cropImage( image, crop );
                cv::imwrite( output_filename, output_mat );

                // add filename for stdio/json output
                output.addOutputFile( output_filename );
            }

            // only loop once unless set to output multiple images (-m)
            if ( ++count == 1 && !config.isOutputMultiple()) break;
        }

    } catch ( cv::Exception e )
    {
        output.errorAndExit( CV_ERROR, e.msg.c_str());
    }

    if ( count == 0 ) output.errorAndExit( NO_FACES, "No faces found in image" );

    return 0;
}

cv::Mat FaceCropper::cropImage( cv::Mat &image, const Bounds &crop )
{
    cv::Mat output_mat = cv::Mat(
            config.getOutputSize().width,
            config.getOutputSize().height,
            CV_8UC1
    );

    cv::Mat cropped = image( cv::Rect( crop.x, crop.y, crop.width, crop.height ));
    cv::resize( cropped, output_mat, output_mat.size(), 0, 0, cv::INTER_CUBIC );

    return output_mat;
}

Bounds &FaceCropper::preventCroppingOutsideImageBounds( Bounds &crop, const cv::Mat &image ) const
{
    if ( crop.x < 0 || crop.y < 0 || crop.y + crop.height > image.rows || crop.x + crop.width > image.cols )
    {
        output.errorAndExit( CROP_ERROR, "Face not within crop bounds" );
    }

    return crop;
}

Bounds &FaceCropper::matchCropToOutputAspectRatio( Bounds &crop, const Bounds &face ) const
{
    float aspect = (float) config.getOutputSize().height /
                   (float) config.getOutputSize().width;

    if ( aspect < 1 )
        crop.height = face.height / aspect;
    else
        crop.width = face.width * aspect;

    return crop;
}

Bounds &FaceCropper::centerCropOnFace( Bounds &crop, const Bounds &face ) const
{
    crop.y = face.y + ( face.height - crop.height ) / 2;
    crop.x = face.x + ( face.width - crop.width ) / 2;
    return crop;
}

Bounds &FaceCropper::addPaddingToCrop( Bounds &crop ) const
{
    float padding = (float) config.getPadding() / 100;
    int   pad_x   = crop.width * padding;
    int   pad_y   = crop.height * padding;

    crop.x -= pad_x;
    crop.y -= pad_y;
    crop.width += pad_x * 2;
    crop.height += pad_y * 2;

    return crop;
}

std::string FaceCropper::generateFilename( const boost::filesystem::path &path, int count, std::string append )
{
    std::string filename = boost::filesystem::canonical( config.getOutputPath()).string() + "/";
    filename += path.stem().string();
    filename += "_" + config.getOutputSuffix();

    if ( config.isOutputMultiple() && count >= 0 )
        filename += "_" + std::to_string( count );

    if ( !append.empty())
        filename += "_" + append;

    if ( config.getOutputFiletype() == Filetype::PNG )
        filename += ".png";
    else if ( config.getOutputFiletype() == Filetype::JPG )
        filename += ".jpg";
    else
        filename += ".jpg";

    return filename;
}

void FaceCropper::ensureInputFileExists()
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

void FaceCropper::ensureOutputPathExists()
{
    fs::path path( config.getOutputPath());

    if ( fs::exists( path ))
    {
        if ( fs::is_directory( path ))
            return;

        output.errorAndExit( PATH_ERROR, "The output path: \'" + path.string() + "\' is not a directory." );
    }

    fs::create_directory( path );
}