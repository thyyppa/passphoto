#ifndef ImageCropper_h
#define ImageCropper_h

#include <string>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Config.h"
#include "ProgramOutput.h"
#include "Bounds.h"

namespace fs = boost::filesystem;

class FaceCropper {
public:
    FaceCropper( Config &config, ProgramOutput &output );
    int cropFaces( FacesDetected &faces );
protected:
    Config        &config;
    ProgramOutput &output;
    void ensureOutputPathExists();
    void ensureInputFileExists();
    std::string generateFilename( const boost::filesystem::path &path, int count, std::string append = "" );
    Bounds &addPaddingToCrop( Bounds &crop ) const;
    Bounds &centerCropOnFace( Bounds &crop, const Bounds &face ) const;
    Bounds &matchCropToOutputAspectRatio( Bounds &crop, const Bounds &face ) const;
    Bounds &preventCroppingOutsideImageBounds( Bounds &crop, const cv::Mat &image ) const;
    cv::Mat cropImage( cv::Mat &image, const Bounds &crop );
};


#endif //ImageCropper_h
