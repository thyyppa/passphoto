#include "ProgramOutput.h"

void ProgramOutput::setDebugFile( const std::string &filename )
{
    debug_file = filename;
}

void ProgramOutput::addOutputFile( std::string filename )
{
    output_files.push_back( filename );
    total_faces_output++;
}

std::string ProgramOutput::toString()
{
    if ( !use_json )
    {
        std::cout << "faces_found: " + std::to_string( total_faces_detected ) + "\n";
        std::cout << "faces_cropped: " + std::to_string( total_faces_output ) + "\n";

        for ( int i = 0; i < output_files.size(); i++ )
            std::cout << "cropped_file[" + std::to_string( i ) + "]: " + output_files[ i ] + "\n";

        if ( !debug_file.empty())
            std::cout << "debug_file: " + debug_file + "\n";

        for ( int i = 0; i < output_bounds.size(); i++ )
        {
            std::cout << "face_bounds[" + std::to_string( i ) + "]" + "_x: " + std::to_string( output_bounds[ i ].face.x ) + "\n";
            std::cout << "face_bounds[" + std::to_string( i ) + "]" + "_y: " + std::to_string( output_bounds[ i ].face.y ) + "\n";
            std::cout << "face_bounds[" + std::to_string( i ) + "]" + "_width: " + std::to_string( output_bounds[ i ].face.width ) + "\n";
            std::cout << "face_bounds[" + std::to_string( i ) + "]" + "_height: " + std::to_string( output_bounds[ i ].face.height ) + "\n";
            std::cout << "crop_bounds[" + std::to_string( i ) + "]" + "_x: " + std::to_string( output_bounds[ i ].crop.x ) + "\n";
            std::cout << "crop_bounds[" + std::to_string( i ) + "]" + "_y: " + std::to_string( output_bounds[ i ].crop.y ) + "\n";
            std::cout << "crop_bounds[" + std::to_string( i ) + "]" + "_width: " + std::to_string( output_bounds[ i ].crop.width ) + "\n";
            std::cout << "crop_bounds[" + std::to_string( i ) + "]" + "_height: " + std::to_string( output_bounds[ i ].crop.height ) + "\n";
        }

        exit( 0 );
    }

    boost::property_tree::ptree json;

    json.put( "faces_found", total_faces_detected );
    json.put( "faces_cropped", total_faces_output );

    for ( int i = 0; i < output_files.size(); i++ )
        json.put( "cropped_images." + std::to_string( i ), output_files[ i ] );

    for ( int i = 0; i < output_bounds.size(); i++ )
    {
        boost::property_tree::ptree bounds_json;
        bounds_json.put( "face.x", output_bounds[ i ].face.x );
        bounds_json.put( "face.y", output_bounds[ i ].face.y );
        bounds_json.put( "face.width", output_bounds[ i ].face.width );
        bounds_json.put( "face.height", output_bounds[ i ].face.height );
        bounds_json.put( "crop.x", output_bounds[ i ].crop.x );
        bounds_json.put( "crop.y", output_bounds[ i ].crop.y );
        bounds_json.put( "crop.width", output_bounds[ i ].crop.width );
        bounds_json.put( "crop.height", output_bounds[ i ].crop.height );
        json.put_child( "bounds." + std::to_string( i ), bounds_json );
    }

    if ( !debug_file.empty())
        json.put( "debug_file", debug_file );

    std::stringstream json_stream;
    boost::property_tree::json_parser::write_json( json_stream, json );

    return json_stream.str();
}

void ProgramOutput::errorAndExit( std::string code, std::string message )
{
    if ( !use_json )
    {
        std::cerr << code << ": " << message << "\n";
        exit( 1 );
    }

    boost::property_tree::ptree json;

    json.put( "error.code", code );
    json.put( "error.message", message );

    std::stringstream json_stream;
    boost::property_tree::json_parser::write_json( json_stream, json );

    std::cerr << json_stream.str();
    exit( 1 );
}

void ProgramOutput::outputAndExit( std::string key, std::string message )
{
    if ( !use_json )
    {
        std::cout << key << ": " << message << "\n";
        exit( 0 );
    }

    boost::property_tree::ptree json;

    json.put( key, message );

    std::stringstream json_stream;
    boost::property_tree::json_parser::write_json( json_stream, json );

    std::cout << json_stream.str();
    exit( 0 );
}

void ProgramOutput::useJson()
{
    use_json = true;
}

void ProgramOutput::addOutputFaceBounds( FaceCropBounds bounds )
{
    output_bounds.push_back( bounds );
    total_faces_detected++;
}
