#include "Reader.h"

reading_and_writing::Reader::Reader(const std::string& filename) {
    path_ = filename;
}

size_t reading_and_writing::GetPaddingSize(size_t width) {
    return (image::utils::PADDING_BYTES - (width * image::utils::BYTES_PER_PIXEL) % image::utils::PADDING_BYTES) %
           image::utils::PADDING_BYTES;
}

Image reading_and_writing::Reader::Read() {
    unsigned char bmp_header[image::utils::BMP_HEADER_SIZE];
    unsigned char dib_header[image::utils::DIB_HEADER_SIZE];

    std::ifstream img;
    img.open(path_, std::ios::in | std::ios::binary);
    if (!img.is_open()) {
        throw std::invalid_argument(std::string("File ") + path_ + std::string(" not found"));
    }
    if (errno == EACCES) {
        throw std::invalid_argument(std::string("NO permission to read file ") + path_);
    }
    img.read(reinterpret_cast<char*>(bmp_header), image::utils::BMP_HEADER_SIZE);
    if (bmp_header[0] != image::utils::HEADER_SIGNATURE[0] || bmp_header[1] != image::utils::HEADER_SIGNATURE[1]) {
        throw std::invalid_argument(std::string("File ") + path_ + std::string(" is not a BMP file"));
    }
    try {
        img.read(reinterpret_cast<char*>(dib_header), image::utils::DIB_HEADER_SIZE);
        size_t width = BytesToRead(dib_header + image::utils::HEADER_WIDTH_OFFSET);
        size_t height = BytesToRead(dib_header + image::utils::HEADER_HEIGHT_OFFSET);
        Image image(width, height);

        std::vector<std::vector<Color>> data(height);
        std::vector<Color> row(width);

        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                unsigned char pix[image::utils::BYTES_PER_PIXEL];
                img.read(reinterpret_cast<char*>(pix), image::utils::BYTES_PER_PIXEL);
                row[j].SetVals(static_cast<uint8_t>(pix[0]), static_cast<uint8_t>(pix[1]),
                               static_cast<uint8_t>(pix[2]));
            }
            data[i] = row;
            img.ignore(static_cast<std::streamsize>(GetPaddingSize(width)));
        }
        std::reverse(data.begin(), data.end());
        image = Image(data);
        img.close();
        return image;
    } catch (const std::exception& e) {
        throw std::string("Error while reading file") + static_cast<std::string>(e.what());
    }
}

size_t reading_and_writing::Reader::BytesToRead(unsigned char* bytes) const {
    size_t number = *bytes;
    for (size_t i = 0; i < image::utils::SHIFT_BITS.size(); ++i) {
        number += *(bytes + i + 1) << image::utils::SHIFT_BITS[i];
    }
    return number;
}
