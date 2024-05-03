#ifndef MAIN_H
#define MAIN_H

#include <iostream>

#include "Filters/Filters.h"
#include "Image/Image.h"
#include "Parser/Parser.h"
#include "Reading_and_writing/Reader.h"
#include "Reading_and_writing/Writer.h"

std::vector<parser::Token> GetTokens(int argc, char* argv[]);

Image GetImage(const std::string& path);

void WriteImage(const std::string& path, const Image& image);

Image ApplyFilter(Image& image, const std::vector<parser::Token>& tokens);

#endif