//==========================================================================================================
// test_config.cpp - Demonstrates the usage of CConfigFile
//==========================================================================================================
#include "config_file.h"
using namespace std;

CConfigFile config;

// Try changing this to "bravo", and empty string, or some other value
string section = "alpha";

// This is a complex structure
struct complex_t {string s; bool b; double f; int32_t i;};

// We're going to read all of our specs into the structure
struct 
{
    int           voltage_range_lo, voltage_range_hi;
    double        pi;
    int           more_integers[2];
    string        filename;
    bool          flag;
    complex_t     complex;
    CConfigScript serial_ports;
    string        my_key;
    int           some_value;
    vector<int>   int_list;
} spec;


void read_configuration()
{
    if (!config.read("test_spec.txt")) exit(0);

    config.get("voltage_range", &spec.voltage_range_lo, &spec.voltage_range_hi);

    config.get("pi", &spec.pi);

    config.get("more_integers", &spec.more_integers[0], &spec.more_integers[1]);

    config.get("filename", &spec.filename);

    config.get("flag", &spec.flag);

    config.get("int_list", &spec.int_list);

    config.get("complex", "sfbi", &spec.complex.s, &spec.complex.f, &spec.complex.b, &spec.complex.i);

    config.get("serial_ports", &spec.serial_ports);

    config.get("my_key", &spec.my_key);
    config.get("some_value", &spec.some_value);
}


void test_config()
{
    try
    {
        config.set_current_section(section);
        read_configuration();
    }
    catch(const runtime_error& e)
    {
        printf("Configuration error : %s\n", e.what());
        exit(0);
    }

    // Display the configuration values we just read in
    printf("voltage_range = %i, %i\n", spec.voltage_range_lo, spec.voltage_range_hi);
    printf("pi = %1.5lf\n", spec.pi);
    printf("more_integers = 0x%X, 0x%X\n",spec.more_integers[0], spec.more_integers[1]);
    printf("filename = %s\n", spec.filename.c_str());
    printf("flag = %i\n", spec.flag);
    printf("complex = %s, %1.4lf, %i, %i\n\n", spec.complex.s.c_str(), spec.complex.f, spec.complex.b, spec.complex.i);
    printf("int_list = ");
    for (int& value : spec.int_list) printf("%i ", value);
    printf("\n\n");

    // Read and display the "serial_ports" script
    CConfigScript& cs = spec.serial_ports;
    while (cs.get_next_line())
    {
        string device = cs.get_next_token();
        int    speed  = cs.get_next_int();
        printf("device = %s, speed = %i\n", device.c_str(), speed);        
    }
    printf("\n");    

    // Display the values of my_key and some_value for the currently selected section
    printf("with section=%s, my_key = '%s' and some_value = %i\n", section.c_str(), spec.my_key.c_str(), spec.some_value);
}


