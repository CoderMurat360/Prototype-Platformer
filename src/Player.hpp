#pragma once

#include "raylib.h"
#include <vector>

class Platform;
class Item;
class GameManager;

class Player {
    private:
        void Move();
        void Jump();
        void Gravity();
        void platformCollision(const std::vector<Platform>& platforms, float prevX, float prevY);
        void itemCollision(std::vector<Item>& items);

    public:
        float xPos, yPos;
        int width, height;
        float speed = 5.0f;
        float velocity = 0.0f;
        float gravity = 0.4f;
        bool onGround = false;
        bool hasGravity = true;
        Color color;

        Player(float x, float y, int w, int h, Color c);

        void Update(const std::vector<Platform>& platforms, std::vector<Item>& items);
        void Draw();
        Rectangle getRec() const;
};
