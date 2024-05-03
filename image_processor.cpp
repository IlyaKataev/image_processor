#include "Image_processor.h"

std::vector<parser::Token> GetTokens(int argc, char* argv[]) {
    std::vector<parser::Token> tokens = parser::Parse(argc, argv);
    if (tokens.size() < 2) {
        throw std::invalid_argument("Not enough arguments");
    }
    return tokens;
}

Image GetImage(const std::string& path) {
    reading_and_writing::Reader reader(path);
    Image image = reader.Read();
    return image;
}

void WriteImage(const std::string& path, const Image& image) {
    reading_and_writing::Writer writer(path);
    writer.Write(image);
}

Image ApplyFilter(Image& image, const std::vector<parser::Token>& tokens) {
    for (size_t i = 2; i < tokens.size(); ++i) {
        image = filters::GetFilter(tokens[i])->Apply(image);
    }
    return image;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "█   █ █   █   █   █████ █████\n█  ██ ██ ██  █ █  █   █ █\n█ █ █ █ █ █ █████ █     ████\n██  █ █  "
                     " █ █   █ █     █\n█   █ █   █ █   █ █     █████\n\n█████ ████   ███  █   █   █████  ████  ████  "
                     "███  ████\n█   █ █   █ █   █ █   █   █     █     █     █   █ █   █\n█   █ ████  █   █ █   █   "
                     "████  █     █     █   █ ████\n█   █ █     █   █ █   █   █     █     █     █   █ █\n█   █ █      "
                     "███  ███████ █████  ████  ████  ███  █\n\n";

        std::cout << "usage:\n";
        std::cout
            << "{program name} {input file path} {output file path} [-{filter name 1} [filter parameter 1] "
               "[filter parameter 2] ...] [-{filter name 2} [filter parameter 1] [filter parameter 2] ...] ...\n\n";

        std::cout << "filters:\n";
        std::cout << "  -crop [width] [height]\n";
        std::cout << "  -neg\n";
        std::cout << "  -gs\n";
        std::cout << "  -sharp\n";
        std::cout << "  -edge [threshold]\n";
        std::cout << "  -blur [sigma]\n";
        std::cout << "  -pix [pixel size]\n";

        return 0;
    }
    try {
        std::vector<parser::Token> tokens = GetTokens(argc, argv);
        Image image = GetImage(tokens[0].name);
        image = ApplyFilter(image, tokens);
        WriteImage(tokens[1].name, image);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
