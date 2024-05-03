#ifndef CPP_HSE_IMAGE_H
#define CPP_HSE_IMAGE_H

#include <stdexcept>
#include <vector>

#include "Color.h"

class Image {
public:
    Image() = default;
    Image(std::size_t width, std::size_t height);
    explicit Image(const std::vector<std::vector<Color>>& data);
    Image(const Image& other);
    ~Image() = default;

    std::size_t GetWidth() const;
    std::size_t GetHeight() const;
    const std::vector<std::vector<Color>>& GetData() const;
    const Color& GetColor(std::size_t x, std::size_t y) const;
    Color& GetColor(std::size_t x, std::size_t y);

    void SetColor(std::size_t x, std::size_t y, const Color& color);

private:
    std::size_t width_ = 0;
    std::size_t height_ = 0;
    std::vector<std::vector<Color>> pixels_;
    void CheckWidthAndHeight(std::size_t width, std::size_t height) const;
};

#endif  // CPP_HSE_IMAGE_H
