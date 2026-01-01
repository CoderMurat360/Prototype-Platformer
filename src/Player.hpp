#pragma once

#include "raylib.h"
#include "raymath.h"
#include <vector>

class Platform;
class Item;
class GameManager;

class Player {
    private:
        float velocityX = 0.0f;     // horizontal velocity
        float velocityY = 0.0f;     // vertical velocity (used for jumping)

        float acceleration = 2000.0f;   // how fast Player speeds up
        float deceleration = 2500.0f;   // how fast Player slows down
        float maxSpeed = 300.0f;        // max horizontal speed

        // all of these are in pixels per second
        const float gravity = 1800.0f;        // gravity will be per second
        float jumpVelocity = -650.0f;   // jump power
        const float terminalVelocity = 1200.0f;   // max falling speed

        void Move(float dt);
        void Jump();
        void Gravity(float dt);
        void platformCollision(const std::vector<Platform>& platforms, float prevX, float prevY);
        void itemCollision(std::vector<Item>& items);

    public:
        float xPos, yPos;
        int width, height;
        bool onGround = false;
        bool hasGravity = true;
        Color color;

        Player(float x, float y, int w, int h, Color c);

        void Update(const std::vector<Platform>& platforms, std::vector<Item>& items);
        void Draw();
        Rectangle getRec() const;
};
