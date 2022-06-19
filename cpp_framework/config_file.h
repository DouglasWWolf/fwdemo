//==========================================================================================================
// config_file.h - Defines a parser for configuration/settings files
//==========================================================================================================
#pragma once
#include <string>
#include <vector>
#include <map>




class CConfigFile
{

public:

    // Call this to read the config file.  Returns 'true' on success, 'false' if file not found
    bool    read(std::string filename, bool msg_on_fail = true);

    // Dumps out the m_specs in a human-readable form.  This is strictly for testing
    void    dump_specs();

protected:

    // A strvec_t is a vector of strings
    typedef std::vector< std::string > strvec_t;

    // Our configuration specs are a vector of spec_t objects
    std::map<std::string, strvec_t> m_specs;
};




