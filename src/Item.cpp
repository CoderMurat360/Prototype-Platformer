#include "Item.hpp"

Item::Item(Vector2 v, int r, Color c) {
    center = v;
    radius = r;
    color = c; 
}

void Item::Draw() {
    if (collected == false) {
        DrawCircleV(center, radius, color);
    }
}
