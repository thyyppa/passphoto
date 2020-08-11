#include "Config.h"

/**
 * This method handles all of the flag and option arguments and then
 * the Config object gets passed around throughout the program.
 *
 * This is also where you will find the backbone of the help text,
 * it's generated using boost::program_options
 *
 * @param argc
 * @param argv
 * @param output
 */
Config::Config( int argc, char *argv[], ProgramOutput &output ) : output( output )
{
    this->output = output;

    // set up options and flags
    po::options_description options(
            makeHelpHeader( HELP_OUTPUT_WIDTH ) +
            "\n\n"
            "Usage: ./" PROJECT_NAME " <options> [input-file]\n\n"
            "Options",
            HELP_OUTPUT_WIDTH,
            HELP_OUTPUT_WIDTH
    );

    options.add_options()(
            "dimensions,d",
            po::value<std::string>(),
            "Set output image dimensions [w]x[h] (default 600x800)"
    )(
            "suffix,s",
            po::value<std::string>( &this->output_suffix ),
            "Set output filename suffix (default 'cropped')"
    )(
            "type,t",
            po::value<std::string>(),
            "Set output file type - JPG or PNG (default JPG)"
    )(
            "cascade,c",
            po::value<std::string>( &this->cascade_path ),
            "OpenCV cascade file \n(default: \"" DEFAULT_CASCADE "\")"
    )(
            "padding,p",
            po::value<int>( &this->padding ),
            "Percent padding relative to face size (default 10)"
    )(
            "multi,m", "Allow multiple faces to be cropped"
    )(
            "input-file,i", po::value<std::string>()->composing(), "Path to original image file"
    )(
            "output-path,o", po::value<std::string>()->composing(), "Path to output cropped images"
    )(
            "json,j", "JSON Output"
    )(
            "validate,k", "Check that a valid face is in the file without cropping"
    )(
            "version,v", "Show version"
    )(
            "debug", "Output debug images showing face and crop bounds"
    )(
            "help,h", "This help message"
    );

    // set up positional values
    po::positional_options_description positional;
    positional.add( "input-file", -1 );

    // set up program options
    po::variables_map vm;
    po::store(
            po::command_line_parser( argc, argv )
                    .options( options )
                    .positional( positional )
                    .run(),
            vm
    );
    po::notify( vm );

    // handle flags
    if ( vm.count( "help" ))
    {
        if ( system( "CLS" )) system( "clear" );
        std::cout << options;
        exit( 0 );
    }

    if ( vm.count( "json" ))
        this->output.useJson();

    if ( vm.count( "validate" ))
        this->setMode( DETECT_ONLY );

    if ( vm.count( "multi" ))
        this->setOutputMultiple( true );

    if ( vm.count( "version" ))
        this->output.outputAndExit(
                "version",
                "v"  VERSION_MAJOR  "."  VERSION_MINOR "."  VERSION_PATCH
        );

    if ( vm.count( "debug" ))
        this->setDebug( true );

    if ( vm.count( "output-path" ))
        this->setOutputPath( vm[ "output-path" ].as<std::string>());

    if ( vm.count( "input-file" ))
        this->setInputPath( vm[ "input-file" ].as<std::string>());
    else
        this->output.errorAndExit(
                FILE_ERROR,
                "no input file given. usage: ./" PROJECT_NAME " [input-file]"
        );

    if ( vm.count( "dimensions" ))
    {
        // split [w]x[h] format into ints and set output size
        const std::string dim = vm[ "dimensions" ].as<std::string>();
        std::regex        pattern( "([0-9]+)x([0-9]+)" );
        std::smatch       match;

        if ( std::regex_search( dim.begin(), dim.end(), match, pattern ))
            this->setOutputSize( std::stoi( match[ 1 ] ), std::stoi( match[ 2 ] ));
        else
            this->output.errorAndExit(
                    DIMENSION_ERROR,
                    "dimensions must match [h]x[w] format. example: 800x600"
            );
    }

    if ( vm.count( "type" ))
    {
        std::string type = vm[ "type" ].as<std::string>();
        boost::algorithm::to_lower( type );

        if ( type == "png" )
            this->setOutputFiletype( Filetype::PNG );
        else if ( type == "jpg" || type == "jpeg" )
            this->setOutputFiletype( Filetype::JPG );
        else
            this->output.errorAndExit(
                    FILE_ERROR,
                    "output type must be one of JPG or PNG"
            );
    }
}

/**
 * Makes a [width] wide ======[ passphoto v0.0.0 ]====== header
 * Is this really necessary? no. probably not.
 *
 * @param width
 * @return
 */
std::string Config::makeHelpHeader( int width ) const
{
    std::string header = PROJECT_NAME " v"  VERSION_MAJOR  "."  VERSION_MINOR"."  VERSION_PATCH;

    int repeat_divider = ( width - header.length() - 4 ) / 2;
    header = std::string( repeat_divider, '=' ) + "[ " + header + " ]" + std::string( repeat_divider, '=' );
    header += "\n" + std::string( width, '=' );

    return header;
}

/**
 * Only getters and setters galore below...
 */

int Config::getPadding() const
{
    return padding;
}

void Config::setPadding( int padding )
{
    padding = padding;
}

const std::string &Config::getCascadePath() const
{
    return cascade_path;
}

void Config::setCascadePath( const std::string &cascade )
{
    Config::cascade_path = cascade;
}

const std::string &Config::getOutputSuffix() const
{
    return output_suffix;
}

void Config::setOutputSuffix( const std::string &outputSuffix )
{
    output_suffix = outputSuffix;
}

const std::string &Config::getOutputPath() const
{
    return output_path;
}

void Config::setOutputPath( const std::string &outputPath )
{
    output_path = outputPath;
}

bool Config::isOutputMultiple() const
{
    return output_multiple;
}

void Config::setOutputMultiple( bool outputMultiple )
{
    output_multiple = outputMultiple;
}

const ImageDimension &Config::getOutputSize() const
{
    return output_size;
}

void Config::setOutputSize( int width, int height )
{
    output_size = ImageDimension{ height, width };
}

Filetype Config::getOutputFiletype() const
{
    return output_filetype;
}

void Config::setOutputFiletype( Filetype outputFiletype )
{
    output_filetype = outputFiletype;
}

const std::string &Config::getInputPath() const
{
    return input_path;
}

void Config::setInputPath( const std::string &inputPath )
{
    input_path = inputPath;
}

bool Config::isDebug() const
{
    return debug;
}

void Config::setDebug( bool debug )
{
    Config::debug = debug;
}

void Config::setMode( OutputMode mode )
{
    Config::mode = mode;
}

OutputMode Config::getMode() const
{
    return mode;
}


