#include "Color.h"

Color::Color(uint8_t blue_in, uint8_t green_in, uint8_t red_in) : blue(blue_in), green(green_in), red(red_in) {
}
void Color::SetVals(uint8_t blue_in, uint8_t green_in, uint8_t red_in) {
    blue = blue_in;
    green = green_in;
    red = red_in;
}
