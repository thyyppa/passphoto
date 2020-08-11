#ifndef ProgramOutput_h
#define ProgramOutput_h

#include <vector>
#include <string>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional.hpp>
#include <iostream>
#include "ErrorTypes.h"
#include "Bounds.h"

class ProgramOutput {
public:
    void setDebugFile( const std::string &filename );
    void addOutputFile( std::string filename );
    void addOutputFaceBounds( FaceCropBounds bounds );
    void errorAndExit( std::string code, std::string message );
    void outputAndExit( std::string key, std::string message );
    void useJson();
    std::string toString();
protected:
    int                         total_faces_detected = 0;
    int                         total_faces_output   = 0;
    bool                        use_json             = false;
    std::vector<std::string>    output_files         = {};
    std::vector<FaceCropBounds> output_bounds        = {};
    std::string                 debug_file;
};


#endif //ProgramOutput_h
