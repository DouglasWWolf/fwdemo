//==========================================================================================================
// config_file.cpp - Implements a parser for configuration/settings files
//==========================================================================================================
#include <cstdio>
#include <string.h>
#include "config_file.h"
using namespace std;


//==========================================================================================================
// cleanup() - Given a line of text, this converts tabs to spaces and strips out CR and LF characters
//==========================================================================================================
static void cleanup(char* input)
{
    while (*input)
    {
        if (*input == '\t') *input = ' ';
        if (*input == '\r') *input = 0;
        if (*input == '\n') *input = 0;
        ++input;
    }
}
//==========================================================================================================


//==========================================================================================================
// parse_to_delimeter() - Returns a string of characters up to (but not including) a space or a delimeter
//
// Passed: in = Pointer the first character of the string
//
// Returns: The parsed string
//==========================================================================================================
static string parse_to_delimeter(const char* in, char delimeter)
{
    char token[256], *out = token;

    // Skip past any leading spaces
    while (*in == ' ') ++in;

    // Fetch the string of characters, terminating at space, end-of-line, or delimeter
    while (*in && *in != ' ' && *in != delimeter) *out++ = *in++;

    // nul-terminate the output string
    *out = 0;

    // Hand the caller the token
    return token;
}
//==========================================================================================================


//==========================================================================================================
// parse_tokens() - Parses an input string into a vector of tokens
//==========================================================================================================
static vector<string> parse_tokens(const char* in)
{
    vector<string> result;
    char           token[256];

    // If we weren't given an input string, return an empty result;
    if (in == nullptr) return result;

    // So long as there are input characters still to be processed...
    while (*in)
    {
        // Point to the output buffer 
        char* out = token;

        // Skip over any leading spaces on the input
        while (*in == ' ') in++;

        // If we hit end-of-line, there are no more tokens to parse
        if (*in == 0) break;

        // Assume for the moment that we're not starting a quoted string
        char in_quotes = 0;

        // If this is a single or double quote-mark, remember it and skip past it
        if (*in == '"' || *in == '\'') in_quotes = *in++;

        // Loop until we've parsed this entire token...
        while (*in)
        {
            // If we're parsing a quoted string...
            if (in_quotes)
            {
                // If we've hit the ending quote-mark, we're done parsing this token
                if (*in == in_quotes)
                {
                    ++in;
                    break;
                }
            }

            // Otherwise, we're not parsing a quoted string. A space or comma ends the token
            else if (*in == ' ' || *in == ',') break;

            // Append this character to the token buffer
            *out++ = *in++;
        }

        // nul-terminate the token string
        *out = 0;

        // Add the token to our result list
        result.push_back(token);

        // Skip over any trailing spaces in the input
        while (*in == ' ') ++in;

        // If there is a trailing comma, throw it away
        if (*in == ',') ++in;
    }

    // Hand the caller a vector of tokens
    return result;
}
//==========================================================================================================



//==========================================================================================================
// Call this to read the config file.  Returns 'true' on success, 'false' if file not found
//==========================================================================================================
bool CConfigFile::read(string filename, bool msg_on_fail)
{
    char     line[1000];
    strvec_t values;

    // This will contain the current [section_name] being parsed
    string current_section;

    // Open the input file
    FILE* ifile = fopen(filename.c_str(), "r");

    // If the input file couldn't be opened, complain about it
    if (ifile == nullptr)
    {
        if (msg_on_fail) printf("Failed to open file \"%s\"\n", filename.c_str());
        return false;        
    }

    // Loop through every line of the input file...
    while (fgets(line, sizeof line, ifile))
    {
        // Convert tabs to spaces and strip out end-of-line characters
        cleanup(line);

        // Find the first non-space character in the line
        char* p = line;
        while (*p == ' ') ++p;

        // If the line is blank or is a comment, ignore it
        if (*p == 0 || *p == '#' || (p[0] == '/' && p[1] == '/')) continue;

        // If the line begins with '[', this is a section-name
        if (*p == '[')
        {
            current_section = parse_to_delimeter(p+1, ']');
            continue;
        }

        // Fetch the base name of this key 
        string base_key_name = parse_to_delimeter(p, '=');


        // Create the fully scoped name of this key
        string scoped_key_name = current_section + "::" + base_key_name;

        // We start out without a list of values for this key
        values.clear();

        // Find the equal sign on this line
        p = strchr(p, '=');

        // If it exists, parse the rest of the line after an '=' into a vector of string tokens    
        if (p) values = parse_tokens(p+1);

        // Add this configuration spec to our master list of config specs
        m_specs[scoped_key_name] = values;
       
    }

    // Close the input file and tell the caller we're done
    fclose(ifile);
    return true;
}
//==========================================================================================================



void CConfigFile::dump_specs()
{
    for (auto e : m_specs)
    {
        printf("Key \"%s\"\n", e.first.c_str());
        for (auto& s  : e.second) printf("   \"%s\"\n", s.c_str());
    }

}


