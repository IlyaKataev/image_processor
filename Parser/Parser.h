#ifndef CPP_HSE_PARSER_H
#define CPP_HSE_PARSER_H

#include <vector>
#include <string>

namespace parser {
class Token {
public:
    std::string name;
    std::vector<std::string> args;
    void Clear();
    bool Empty() const;
};

std::vector<Token> Parse(int argc, char** argv);
}  // namespace parser

#endif  // CPP_HSE_PARSER_H
