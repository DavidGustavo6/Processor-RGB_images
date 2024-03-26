#include "XPM2.hpp"
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

namespace prog {
    int hexadecimal_para_decimal(std::string hexadecimal) {
    int resultado1 = 0;
    int length = hexadecimal.length();

    for (int i = 0; i < length; i++) {
        char c = hexadecimal[i];
        int resultado;
        if (c >= '0' && c <= '9') {
            resultado = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            resultado = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            resultado = c - 'a' + 10;
        }
        resultado1 = (resultado1 * 16) + resultado;
    }
    return resultado1;
}

Image* loadFromXPM2(const std::string& file) {
    std::ifstream input(file);
    if (!input.is_open()) {
        return nullptr;
    }
    std::string line;
    getline(input, line);
    getline(input, line);
    int w, h, n;
    std::istringstream iss(line);
    iss >> w >> h >> n;
    Image* imagemnova = new Image(w, h);
    std::map<char, Color> mapeamento_de_cores;
    for (int i = 0; i < n; i++) {
        getline(input, line);
        char simbolo;
        std::istringstream iss(line);
        std::string c, hexcode;
        iss >> simbolo;
        iss >> c;
        iss >> hexcode;
        Color cornova;
        cornova.red() = hexadecimal_para_decimal(hexcode.substr(1, 2));
        cornova.green() = hexadecimal_para_decimal(hexcode.substr(3, 2));
        cornova.blue() = hexadecimal_para_decimal(hexcode.substr(5, 2));
        mapeamento_de_cores[simbolo] = cornova;
    }
    for (int y = 0; y < h; y++) {
        getline(input, line);
        std::istringstream iss(line);
        std::string var;
        iss >> var;
        for (int x = 0; x < w; x++) {
            Color cor = mapeamento_de_cores[var[x]];
            imagemnova->at(x, y).red() = cor.red();
            imagemnova->at(x, y).green() = cor.green();
            imagemnova->at(x, y).blue() = cor.blue();
        }
    }
    input.close();
    return imagemnova;
}

std::string decimal_para_hexadecimal(int decimal) {
    std::string hexadecimal;
    while (decimal > 0) {
        int remainder = decimal % 16;
        char digit;
        if (remainder < 10) {
            digit = '0' + remainder;
        } else {
            digit = 'A' + (remainder - 10);
        }
        hexadecimal = digit + hexadecimal;
        decimal /= 16;
    }

    if (hexadecimal.empty()) {
        hexadecimal = "00";
    } else if (hexadecimal.length() == 1) {
        hexadecimal = "0" + hexadecimal;
    }
    return hexadecimal;
}


    void saveToXPM2(const std::string& file, const Image* image) {
    std::ofstream output(file);
    if (!output.is_open()) {
        return;
    }

    output << "! XPM2\n";
    int width = image->width();
    int height = image->height();

    std::unordered_map<Color, std::string> uniqueColors;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color color = image->at(x, y);
            uniqueColors[color] = "";
        }
    }
    int numColors = uniqueColors.size();

    output << width << " " << height << " " << numColors << " 1\n";

    int symbolIndex = 0;
    for (const auto& entry : uniqueColors) {
        const Color& color = entry.first;
        std::string hexColor = "#" +
            decimal_to_hexadecimal(color.red()) +
            decimal_to_hexadecimal(color.green()) +
            decimal_to_hexadecimal(color.blue());

        std::string symbol = " " + hexColor;
        uniqueColors[color] = symbol;
        output << symbol << " c " << hexColor << "\n";
        symbolIndex++;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color color = image->at(x, y);
            std::string symbol = uniqueColors[color];
            output << symbol;
        }
        output << "\n";
    }

    output.close();
}


    
}
