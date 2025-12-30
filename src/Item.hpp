#pragma once

#include "raylib.h"

class Item {
public:
    Vector2 center;
    int radius;
    bool collected = false;
    Color color;

    Item(Vector2 v, int r, Color c);
    void Draw();
};
