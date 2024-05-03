#include "Filters.h"

template <typename T>
std::vector<T> filters::Filter::ApplyFilterToPixel(const std::vector<std::vector<T>>& matrix, const Image& image,
                                                   size_t x, size_t y) const {
    T blue = 0;
    T green = 0;
    T red = 0;

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix.front().size(); ++j) {
            size_t row_index = std::min((x + i >= 1) ? x - 1 + i : 0, image.GetHeight() - 1);
            size_t column_index = std::min((y + j >= 1) ? y - 1 + j : 0, image.GetWidth() - 1);
            Color color = image.GetColor(row_index, column_index);
            blue += color.blue * matrix[i][j];
            green += color.green * matrix[i][j];
            red += color.red * matrix[i][j];
        }
    }
    blue =
        std::clamp(blue, static_cast<T>(image::utils::MIN_COLOR_VALUE), static_cast<T>(image::utils::MAX_COLOR_VALUE));
    green =
        std::clamp(green, static_cast<T>(image::utils::MIN_COLOR_VALUE), static_cast<T>(image::utils::MAX_COLOR_VALUE));
    red = std::clamp(red, static_cast<T>(image::utils::MIN_COLOR_VALUE), static_cast<T>(image::utils::MAX_COLOR_VALUE));
    return {blue, green, red};
}

Image filters::Crop::Apply(const Image& image) const {
    size_t new_width = std::min(image.GetWidth(), width_);
    size_t new_height = std::min(image.GetHeight(), height_);
    std::vector<std::vector<Color>> new_image_data(new_height, std::vector<Color>(new_width));
    for (size_t i = 0; i < new_height; ++i) {
        std::vector<Color> row(new_width);
        for (size_t j = 0; j < new_width; ++j) {
            row[j] = image.GetColor(i, j);
        }
        new_image_data[i] = row;
    }
    return Image(new_image_data);
}

Image filters::Negative::Apply(const Image& image) const {
    std::vector<std::vector<Color>> new_image_data(image.GetHeight(), std::vector<Color>(image.GetWidth()));
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            Color color = image.GetColor(i, j);
            row[j].SetVals(static_cast<uint8_t>(image::utils::MAX_COLOR_VALUE - color.blue),
                           static_cast<uint8_t>(image::utils::MAX_COLOR_VALUE - color.green),
                           static_cast<uint8_t>(image::utils::MAX_COLOR_VALUE - color.red));
        }
        new_image_data[i] = row;
    }
    return Image(new_image_data);
}

Image filters::Grayscale::Apply(const Image& image) const {
    std::vector<std::vector<Color>> new_image_data(image.GetHeight(), std::vector<Color>(image.GetWidth()));
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            Color color = image.GetColor(i, j);
            uint8_t gray =
                static_cast<uint8_t>(image::utils::RED_FACTOR * color.red + image::utils::GREEN_FACTOR * color.green +
                                     image::utils::BLUE_FACTOR * color.blue);
            row[j].SetVals(gray, gray, gray);
        }
        new_image_data[i] = row;
    }
    return Image(new_image_data);
}

Image filters::Sharpening::Apply(const Image& image) const {
    std::vector<std::vector<Color>> new_image_data(image.GetHeight(), std::vector<Color>(image.GetWidth()));
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            std::vector<int> pixel_colors = ApplyFilterToPixel<int>(matrix, image, i, j);
            row[j].SetVals(pixel_colors[0], pixel_colors[1], pixel_colors[2]);
        }
        new_image_data[i] = row;
    }
    return Image(new_image_data);
}

Image filters::Edge::Apply(const Image& image) const {
    std::vector<std::vector<Color>> new_image_data(image.GetHeight(), std::vector<Color>(image.GetWidth()));
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    Image grayscale_image = filters::Grayscale().Apply(image);
    for (size_t i = 0; i < grayscale_image.GetHeight(); ++i) {
        std::vector<Color> row(grayscale_image.GetWidth());
        for (size_t j = 0; j < grayscale_image.GetWidth(); ++j) {
            std::vector<int> pixel_colors = ApplyFilterToPixel<int>(matrix, grayscale_image, i, j);
            double pixel_color_value = pixel_colors[0];
            if (static_cast<double>(pixel_color_value) > image::utils::MAX_COLOR_VALUE * threshold_) {
                row[j].SetVals(image::utils::MAX_COLOR_VALUE, image::utils::MAX_COLOR_VALUE,
                               image::utils::MAX_COLOR_VALUE);
            } else {
                row[j].SetVals(image::utils::MIN_COLOR_VALUE, image::utils::MIN_COLOR_VALUE,
                               image::utils::MIN_COLOR_VALUE);
            }
        }
        new_image_data[i] = row;
    }
    return Image(new_image_data);
}

Image filters::Blur::Apply(const Image& image) const {
    std::vector<std::vector<Color>> temp_image_data(image.GetHeight(), std::vector<Color>(image.GetWidth()));
    std::vector<std::vector<Color>> new_image_data(image.GetHeight(), std::vector<Color>(image.GetWidth()));

    int kernel_size = static_cast<int>(std::ceil(sigma_ * 3)) * 2 + 1;
    std::vector<float> kernel(kernel_size);

    float sum = 0;
    int half_kernel_size = kernel_size / 2;
    for (int i = -half_kernel_size; i <= half_kernel_size; ++i) {
        float value = std::exp(-(static_cast<float>(i) * static_cast<float>(i)) / (2 * sigma_ * sigma_));
        kernel[i + half_kernel_size] = value;
        sum += value;
    }

    for (int i = 0; i < kernel_size; ++i) {
        kernel[i] /= sum;
    }

    // horizontal blur
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            float blue = 0;
            float green = 0;
            float red = 0;
            for (int k = -half_kernel_size; k <= half_kernel_size; ++k) {
                int column = std::clamp(static_cast<int>(j) + k, 0, static_cast<int>(image.GetWidth()) - 1);
                Color color = image.GetColor(i, column);
                blue += static_cast<float>(color.blue) * kernel[k + half_kernel_size];
                green += static_cast<float>(color.green) * kernel[k + half_kernel_size];
                red += static_cast<float>(color.red) * kernel[k + half_kernel_size];
            }
            row[j].SetVals(static_cast<uint8_t>(blue), static_cast<uint8_t>(green), static_cast<uint8_t>(red));
        }
        temp_image_data[i] = row;
    }

    // vertical blur
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            float blue = 0;
            float green = 0;
            float red = 0;
            for (int k = -half_kernel_size; k <= half_kernel_size; ++k) {
                int row_index = std::clamp(static_cast<int>(i) + k, 0, static_cast<int>(image.GetHeight()) - 1);
                Color color = temp_image_data[row_index][j];
                blue += static_cast<float>(color.blue) * kernel[k + half_kernel_size];
                green += static_cast<float>(color.green) * kernel[k + half_kernel_size];
                red += static_cast<float>(color.red) * kernel[k + half_kernel_size];
            }
            row[j].SetVals(static_cast<uint8_t>(blue), static_cast<uint8_t>(green), static_cast<uint8_t>(red));
        }
        new_image_data[i] = row;
    }

    return Image(new_image_data);
}

Image filters::Pixellate::Apply(const Image& image) const {
    std::vector<std::vector<Color>> new_image_data(image.GetHeight(), std::vector<Color>(image.GetWidth()));
    for (size_t i = 0; i < image.GetHeight(); i += pixel_size_) {
        for (size_t j = 0; j < image.GetWidth(); j += pixel_size_) {
            Color color = image.GetColor(i, j);
            for (size_t k = i; k < std::min(i + pixel_size_, image.GetHeight()); ++k) {
                for (size_t l = j; l < std::min(j + pixel_size_, image.GetWidth()); ++l) {
                    new_image_data[k][l] = color;
                }
            }
        }
    }
    return Image(new_image_data);
}

std::unique_ptr<filters::Filter> filters::GetFilter(const parser::Token& token) {
    const std::string& name = token.name;
    if (name == "-crop") {
        if (token.args.size() != 2) {
            throw std::invalid_argument("Crop filter requires exactly two arguments");
        }
        try {
            size_t width = std::stoul(token.args[0]);
            size_t height = std::stoul(token.args[1]);
            return std::make_unique<filters::Crop>(width, height);
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Crop filter requires non-negative integer arguments");
        }
    } else if (name == "-gs") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Grayscale filter doesn't take any arguments");
        }
        return std::make_unique<filters::Grayscale>();
    } else if (name == "-neg") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Negative filter doesn't take any arguments");
        }
        return std::make_unique<filters::Negative>();
    } else if (name == "-sharp") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Sharpening filter doesn't take any arguments");
        }
        return std::make_unique<filters::Sharpening>();
    } else if (name == "-edge") {
        if (token.args.size() != 1) {
            throw std::invalid_argument("Edge filter requires exactly one argument");
        }
        try {
            float threshold = std::stof(token.args[0]);
            if (threshold < 0 || threshold > 1) {
                throw std::invalid_argument("Edge filter requires a threshold between 0 and 1");
            }
            return std::make_unique<filters::Edge>(threshold);
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Edge filter requires a numeric threshold");
        }
    } else if (name == "-blur") {
        if (token.args.size() != 1) {
            throw std::invalid_argument("Blur filter requires exactly one argument");
        }
        try {
            float sigma = std::stof(token.args[0]);
            return std::make_unique<filters::Blur>(sigma);
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Blur filter requires a numeric sigma");
        }
    } else if (name == "-pix") {
        if (token.args.size() != 1) {
            throw std::invalid_argument("Pixellate filter requires exactly one argument");
        }
        try {
            size_t pixel_size = std::stoul(token.args[0]);
            return std::make_unique<filters::Pixellate>(pixel_size);
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Pixellate filter requires a non-negative integer pixel size");
        }
    }
    throw std::runtime_error("Invalid token");
}
