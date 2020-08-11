#ifndef Config_h
#define Config_h

#include <string>
#include <stdlib.h>
#include <iostream>
#include <regex>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include "Version.h"
#include "ProgramOutput.h"

namespace po = boost::program_options;

#define DEFAULT_PADDING             10
#define DEFAULT_CASCADE             "/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml"
#define DEFAULT_OUTPUT_SUFFIX       "cropped"
#define DEFAULT_OUTPUT_PATH         "./"
#define DEFAULT_OUTPUT_MULTIPLE     false
#define DEFAULT_DEBUG               false
#define DEFAULT_OUTPUT_SIZE_HEIGHT  800
#define DEFAULT_OUTPUT_SIZE_WIDTH   600
#define DEFAULT_OUTPUT_FILETYPE     JPG
#define DEFAULT_OUTPUT_MODE         DETECT_AND_CROP
#define HELP_OUTPUT_WIDTH           80

enum Filetype { JPG, PNG };
enum OutputMode { DETECT_AND_CROP, DETECT_ONLY };
struct ImageDimension {
    int width, height;
};

class Config {
protected:
    std::string    input_path;
    std::string    cascade_path    = DEFAULT_CASCADE;
    std::string    output_suffix   = DEFAULT_OUTPUT_SUFFIX;
    std::string    output_path     = DEFAULT_OUTPUT_PATH;
    int            padding         = DEFAULT_PADDING;
    bool           debug           = DEFAULT_DEBUG;
    bool           output_multiple = DEFAULT_OUTPUT_MULTIPLE;
    Filetype       output_filetype = DEFAULT_OUTPUT_FILETYPE;
    OutputMode     mode            = DEFAULT_OUTPUT_MODE;
public:
    OutputMode getMode() const;
protected:
    ImageDimension output_size     = { DEFAULT_OUTPUT_SIZE_HEIGHT, DEFAULT_OUTPUT_SIZE_WIDTH };
public:
    void setMode( OutputMode mode );
public:
    Config( int argc, char *argv[], ProgramOutput &output );

    const std::string &getInputPath() const;
    void setInputPath( const std::string &inputPath );

    int getPadding() const;
    void setPadding( int padding );

    const std::string &getCascadePath() const;
    void setCascadePath( const std::string &cascade );

    const std::string &getOutputSuffix() const;
    void setOutputSuffix( const std::string &outputSuffix );

    const std::string &getOutputPath() const;
    void setOutputPath( const std::string &outputPath );

    bool isOutputMultiple() const;
    void setOutputMultiple( bool outputMultiple );

    const ImageDimension &getOutputSize() const;
    void setOutputSize( int width, int height );

    Filetype getOutputFiletype() const;
    void setOutputFiletype( Filetype outputFiletype );

    bool isDebug() const;
    void setDebug( bool debug );

private:
    ProgramOutput &output;

    std::string makeHelpHeader( int width ) const;
};


#endif //Config_h
