#ifndef FaceDetector_h
#define FaceDetector_h

#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Config.h"
#include "ProgramOutput.h"
#include "Bounds.h"

namespace fs = boost::filesystem;

class FaceDetector {
public:
    FaceDetector( Config &config, ProgramOutput &output );
    FacesDetected detectFaces();
protected:
    Config        &config;
    ProgramOutput &output;
    FacesDetected faces;
    void ensureCascadeFileExists();
    void ensureInputFileExists();
};


#endif //FaceDetector_h
