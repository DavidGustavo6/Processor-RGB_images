#include "Image.hpp"
#include "Color.hpp"
#include <vector>
#include <stdexcept>

namespace prog {
    Image::Image(int w, int h, const Color& fill){
      width_ = w; 
      height_ = h; 
      pixel_ = std::vector<Color>(w * h, fill);
    }

    Image::~Image() {
    }

    int Image::width() const { 
      return width_; 
    }

    int Image::height() const { 
      return height_; 
    }

    int& Image::width(){ 
      return width_; 
    }

    int& Image::height(){ 
      return height_; 
    }

    Color& Image::at(int x, int y) {
      return pixel_[y * width_ + x]; 
    }
    
    const Color& Image::at(int x, int y) const { 
      return pixel_[y * width_ + x]; 
    }

}
