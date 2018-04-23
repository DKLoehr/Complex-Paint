#ifndef IO_HELPER_H
#define IO_HELPER_H

#include <string>

 struct ini_data {
  std::string font;
  unsigned int height;
  unsigned int width;
};

// Primitive parser to read options from the ini file.
ini_data parse_ini(std::string ini_name);

void emit_error(std::string msg);

void emit_warning(std::string msg);



#endif // IO_HELPER_H
