#include "iohelpers.h"

#include <iostream>
#include <fstream>
#include "runner.h"

// TODO: Change this to use regexes instead
ini_data parse_ini(std::string ini_name) {
    ini_data options;
    options.font = "VeraMono.ttf";
    options.height = 1200;
    options.width = 724;

    std::ifstream ini_file;
    ini_file.open(ini_name);
    if(!ini_file.is_open()) {
        std::cout << "Failed to open ini file. Using default parameters\n";
        return options;
    }

    std::string line;
    while(std::getline(ini_file, line)) {
        if(line.length() == 0) // Empty line, ignore
            continue;
        if(line[0] == '#') // This is a comment, so ignore it
            continue;
        else if(!line.substr(0, 5).compare("font="))
            options.font = line.substr(5, std::string::npos);
        else if(!line.substr(0, 7).compare("height="))
            options.height = atoi(line.substr(7, std::string::npos).c_str());
        else if(!line.substr(0, 6).compare("width="))
            options.width = atoi(line.substr(6, std::string::npos).c_str());
        else // Unrecognized line, let the user know
            std::cout << "Unrecognized line in the ini file:\n" << line << "\n";
  }
  ini_file.close();
  return options;
}

//TODO: make this do something more than print to console
void emit_error(std::string msg) {
    std::cout << "Error: " << msg;
}

//TODO: make this do something more than print to console
void emit_warning(std::string msg) {
    std::cout << "Warning: " << msg;
}
