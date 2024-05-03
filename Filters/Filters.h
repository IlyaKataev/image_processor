#ifndef FILTERS_H
#define FILTERS_H

#include <algorithm>
#include <cmath>
#include <memory>
#include <stdexcept>

#include "../Image/Image.h"
#include "../Parser/Parser.h"
#include "../Reading_and_writing/Utils.h"

namespace filters {
class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;
    virtual Image Apply(const Image& image) const = 0;

protected:
    template <typename T>
    std::vector<T> ApplyFilterToPixel(const std::vector<std::vector<T>>& matrix, const Image& image, size_t x,
                                      size_t y) const;

private:
    std::vector<std::string> args_;
    template <typename T>
    std::vector<T> ApplyFilterToPixel(const std::vector<T>& kernel, const Image& image, size_t x, size_t y) const;
};

class Negative : public Filter {
public:
    Image Apply(const Image& image) const override;
};

class Grayscale : public Filter {
public:
    Image Apply(const Image& image) const override;
};

class Sharpening : public Filter {
public:
    Image Apply(const Image& image) const override;
};

class Edge : public Filter {
public:
    explicit Edge(double threshold) : threshold_(threshold) {
    }
    Image Apply(const Image& image) const override;

private:
    double threshold_;
};

class Crop : public Filter {
public:
    explicit Crop(size_t width, size_t height) : width_(width), height_(height) {
    }
    Image Apply(const Image& image) const override;

private:
    size_t width_;
    size_t height_;
};

class Blur : public Filter {
public:
    explicit Blur(float sigma) : sigma_(sigma) {
    }
    Image Apply(const Image& image) const override;

private:
    float sigma_;
};

class Pixellate : public Filter {
public:
    explicit Pixellate(size_t pixel_size) : pixel_size_(pixel_size) {
    }
    Image Apply(const Image& image) const override;

private:
    size_t pixel_size_;
};

std::unique_ptr<filters::Filter> GetFilter(const parser::Token& token);
}  // namespace filters

#endif
