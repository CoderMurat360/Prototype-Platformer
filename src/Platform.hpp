#pragma once
#include "raylib.h"

class Platform {
    public:
        float xPos, yPos;
        int width, height;
        Color color; 

        Platform(float x, float y, int w, int h, Color c);

        void Draw();
        Rectangle getRec() const;
};
