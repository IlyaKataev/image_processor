#ifndef CPP_HSE_READER_H
#define CPP_HSE_READER_H

#include <algorithm>
#include <cerrno>
#include <fstream>
#include <string>

#include "Utils.h"
#include "../Image/Image.h"

namespace reading_and_writing {
class Reader {
public:
    explicit Reader(const std::string& filename);
    Image Read();

private:
    std::string path_;
    size_t BytesToRead(unsigned char* bytes) const;
};
size_t GetPaddingSize(size_t width);
}  // namespace reading_and_writing

#endif  // CPP_HSE_READER_H
