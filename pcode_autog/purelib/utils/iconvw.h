#ifndef _ICONV_WRAPPER_H_
#define _ICONV_WRAPPER_H_

#include <iconv.h>
#include <string>

namespace iconvw {

std::string u2g(const std::string& utf8_text);
std::string g2u(const std::string& gb_text);
std::string convert(const std::string& in, const char* fromcode, const char* tocode);
bool        convert(const std::string& in, std::string& out, const char* fromcode, const char* tocode);

};

#endif

