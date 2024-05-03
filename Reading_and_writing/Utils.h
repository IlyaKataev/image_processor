#ifndef CPP_HSE_UTILS_H
#define CPP_HSE_UTILS_H

#include <string>
#include <vector>
#include <iostream>

namespace image::utils {
// Reading and writing
const int BMP_HEADER_SIZE = 14;
const int DIB_HEADER_SIZE = 40;
const int PADDING_BYTES = 4;
const int BYTES_PER_PIXEL = 3;
const int HEADER_WIDTH_OFFSET = 4;
const int HEADER_HEIGHT_OFFSET = 8;
const int HEADER_FILE_SIZE_OFFSET = 2;
const int PIXEL_ARRAY_OFFSET = 10;
const int FILE_FORMAT_FIRST_POSITION = 0;
const int FILE_FORMAT_SECOND_POSITION = 1;
const int INFORMATION_HEADER_SIZE_POSITION = 0;
const int COLOR_PLANES = 1;
const int COLOR_PLANES_POSITION = 12;
const int BITS_PER_PIXEL_POSITION = 14;
const int BITS_PER_PIXEL = 24;
const std::vector<int> SHIFT_BITS = {8, 16, 24};
const std::vector<char> HEADER_SIGNATURE = {'B', 'M'};
// Filters
const double RED_FACTOR = 0.299;
const double GREEN_FACTOR = 0.587;
const double BLUE_FACTOR = 0.114;
const int MAX_COLOR_VALUE = 255;
const int MIN_COLOR_VALUE = 0;
}  // namespace image::utils

#endif  // CPP_HSE_UTILS_H
