#ifndef __prog_XPM2_hpp__
#define __prog_XPM2_hpp__

#include <string>
#include "Image.hpp"

namespace prog {
    int hexadecimal_para_decimal(std::string hexadecimal);
    Image* loadFromXPM2(const std::string &file);
    std::string decimal_para_hexadecimal(int decimal);
    void saveToXPM2(const std::string& file, const Image* img);
}
#endif
