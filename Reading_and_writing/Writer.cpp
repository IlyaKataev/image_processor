#include "Writer.h"

template <typename T>
void reading_and_writing::Writer::WriteBytes(T number, unsigned char *bytes) {
    *bytes = number;
    for (size_t i = 0; i < image::utils::SHIFT_BITS.size(); ++i) {
        *(bytes + i + 1) = number >> image::utils::SHIFT_BITS[i];
    }
}

void reading_and_writing::Writer::WriteBMPHeader(unsigned char *bmp_header, size_t file_size) {
    bmp_header[image::utils::FILE_FORMAT_FIRST_POSITION] = image::utils::HEADER_SIGNATURE[0];
    bmp_header[image::utils::FILE_FORMAT_SECOND_POSITION] = image::utils::HEADER_SIGNATURE[1];
    WriteBytes(file_size, bmp_header + image::utils::HEADER_FILE_SIZE_OFFSET);
    bmp_header[image::utils::PIXEL_ARRAY_OFFSET] = image::utils::BMP_HEADER_SIZE + image::utils::DIB_HEADER_SIZE;
}

void reading_and_writing::Writer::WriteDIBHeader(unsigned char *dib_header, size_t width, size_t height) {
    dib_header[image::utils::INFORMATION_HEADER_SIZE_POSITION] = image::utils::DIB_HEADER_SIZE;
    WriteBytes(width, dib_header + image::utils::HEADER_WIDTH_OFFSET);
    WriteBytes(height, dib_header + image::utils::HEADER_HEIGHT_OFFSET);
    dib_header[image::utils::COLOR_PLANES_POSITION] = image::utils::COLOR_PLANES;
    dib_header[image::utils::BITS_PER_PIXEL_POSITION] = image::utils::BITS_PER_PIXEL;
}

reading_and_writing::Writer::Writer(const std::string path) : path_(std::move(path)) {
}

void reading_and_writing::Writer::Write(const Image &image) {
    std::ofstream out_file;
    out_file.open(path_, std::ios::out | std::ios::binary);
    if (!out_file.is_open()) {
        throw std::invalid_argument(std::string("Can't open file ") + path_);
    }
    if (errno == EACCES) {
        throw std::invalid_argument(std::string("Permission denied to file ") + path_);
    }
    const size_t file_size = image::utils::BMP_HEADER_SIZE + image::utils::DIB_HEADER_SIZE +
                             image.GetHeight() * image.GetWidth() * image::utils::BYTES_PER_PIXEL +
                             GetPaddingSize(image.GetWidth()) * image.GetHeight();

    unsigned char bmp_header[image::utils::BMP_HEADER_SIZE];
    std::fill(bmp_header, bmp_header + image::utils::BMP_HEADER_SIZE, 0);
    WriteBMPHeader(bmp_header, file_size);

    unsigned char dib_header[image::utils::DIB_HEADER_SIZE];
    std::fill(dib_header, dib_header + image::utils::DIB_HEADER_SIZE, 0);
    WriteDIBHeader(dib_header, image.GetWidth(), image.GetHeight());

    out_file.write(reinterpret_cast<char *>(bmp_header), image::utils::BMP_HEADER_SIZE);
    out_file.write(reinterpret_cast<char *>(dib_header), image::utils::DIB_HEADER_SIZE);

    unsigned char empty_pix[image::utils::BYTES_PER_PIXEL] = {0, 0, 0};
    for (size_t i = image.GetHeight(); i-- > 0;) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            uint8_t blue = static_cast<uint8_t>(image.GetColor(i, j).blue);
            uint8_t green = static_cast<uint8_t>(image.GetColor(i, j).green);
            uint8_t red = static_cast<uint8_t>(image.GetColor(i, j).red);
            unsigned char pixel[image::utils::BYTES_PER_PIXEL] = {blue, green, red};
            out_file.write(reinterpret_cast<char *>(pixel), image::utils::BYTES_PER_PIXEL);
        }
        out_file.write(reinterpret_cast<char *>(empty_pix),
                       static_cast<std::streamsize>(GetPaddingSize(image.GetWidth())));
    }
    out_file.close();
}
