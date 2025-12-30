#include "Player.hpp"
#include "Platform.hpp"
#include "Item.hpp"
#include "GameManager.hpp"

// GameManager is global in main.cpp
extern class GameManager gm;

Player::Player(float x, float y, int w, int h, Color c) {
    xPos = x;
    yPos = y;
    width = w;
    height = h;
    color = c;
}

void Player::Move() {
    if (IsKeyDown(KEY_A)) xPos -= speed;
    else if (IsKeyDown(KEY_D)) xPos += speed;
}

void Player::Jump() {
    if (IsKeyDown(KEY_SPACE) && onGround) {
        velocity = -10.0f;
        onGround = false;
    }
}

void Player::Gravity() {
    velocity += gravity;
    yPos += velocity;
}

void Player::platformCollision(const std::vector<Platform>& platforms, float prevX, float prevY) {
    Rectangle playerRec = getRec();

    for (const auto& platform : platforms) {
        Rectangle platRec = platform.getRec();

        if (CheckCollisionRecs(playerRec, platRec)) {
            float prevBottom = prevY + height;
            float platTop = platRec.y;

            if (prevBottom <= platTop) {
                yPos = platTop - height;
                velocity = 0;
                onGround = true;
            }
        }
    }
}

void Player::itemCollision(std::vector<Item>& items) {
    for (auto& i : items) {
        if (!i.collected && CheckCollisionCircleRec(i.center, i.radius, getRec())) {
            i.collected = true;
            gm.updateScore(1);
        }
    }
}

void Player::Update(const std::vector<Platform>& platforms, std::vector<Item>& items) {
    float prevX = xPos;
    float prevY = yPos;

    Jump();
    Gravity();
    Move();

    platformCollision(platforms, prevX, prevY);
    itemCollision(items);
}

void Player::Draw() {
    DrawRectangle((int)xPos, (int)yPos, width, height, color);
}

Rectangle Player::getRec() const {
    return Rectangle{xPos, yPos, (float)width, (float)height};
}
