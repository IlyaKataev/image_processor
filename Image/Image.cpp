#include "Image.h"

Image::Image(std::size_t width, std::size_t height)
    : width_(width), height_(height), pixels_(height, std::vector<Color>(width)) {
}

Image::Image(const std::vector<std::vector<Color>>& data)
    : width_(data[0].size()), height_(data.size()), pixels_(data) {
}

Image::Image(const Image& other) : width_(other.width_), height_(other.height_), pixels_(other.pixels_) {
}

std::size_t Image::GetWidth() const {
    return width_;
}

std::size_t Image::GetHeight() const {
    return height_;
}

const std::vector<std::vector<Color>>& Image::GetData() const {
    return pixels_;
}

const Color& Image::GetColor(std::size_t x, std::size_t y) const {
    if (x >= height_ || y >= width_) {
        throw std::out_of_range("Invalid coordinates");
    }
    return pixels_[x][y];
}

Color& Image::GetColor(std::size_t x, std::size_t y) {
    if (x >= height_ || y >= width_) {
        throw std::out_of_range("Invalid coordinates");
    }
    return pixels_[x][y];
}

void Image::SetColor(std::size_t x, std::size_t y, const Color& color) {
    if (x >= height_ || y >= width_) {
        throw std::out_of_range("Invalid coordinates");
    }
    pixels_[x][y] = color;
}
void Image::CheckWidthAndHeight(std::size_t width, std::size_t height) const {
    if (width == 0 || height == 0) {
        throw std::invalid_argument("Width and height must be greater than 0");
    }
}
