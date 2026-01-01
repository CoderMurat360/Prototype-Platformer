#include "Platform.hpp"

Platform::Platform(float x, float y, int w, int h, Color c) :
    xPos(x),
    yPos(y),
    width(w),
    height(h),
    color(c)
{}

void Platform::Draw() {
    DrawRectangle((int)xPos, (int)yPos, width, height, color);
}

Rectangle Platform::getRec() const {
    return Rectangle{xPos, yPos, (float)width, (float)height};
}
