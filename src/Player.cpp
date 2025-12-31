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

void Player::Move(float dt) {
    float input = 0.0f;

    if (IsKeyDown(KEY_A)) { input -= 1.0f; }
    if (IsKeyDown(KEY_D)) { input += 1.0f; }

    // if Player is moving
    if (input != 0.0f) {
        // accelerate
        velocityX += input * acceleration * dt;
    } else {
        // decelerate (snappy stop, no ice)
        if (velocityX > 0) {
            velocityX -= deceleration * dt;
            if (velocityX < 0) velocityX = 0;
        } else if (velocityX < 0) {
            velocityX += deceleration * dt;
            if (velocityX > 0) velocityX = 0;
        }
    }

    // clamp speed
    if (velocityX > maxSpeed) { velocityX = maxSpeed; }
    if (velocityX < -maxSpeed) { velocityX = -maxSpeed; }

    // apply movement
    xPos += velocityX * dt;
}


void Player::Jump() {
    if (IsKeyPressed(KEY_SPACE) && onGround) {
        velocityY = jumpVelocity;
        onGround = false;
    }
}

void Player::Gravity(float dt) {
    velocityY += gravity * dt;
    velocityY = Clamp(velocityY, -10000.0f, terminalVelocity);
    yPos += velocityY * dt;
}

void Player::platformCollision(const std::vector<Platform>& platforms, float prevX, float prevY) {
    // get player rectangle data
    Rectangle playerRec = getRec();

    // go through all of the platforms to check
    for (const auto& platform : platforms) {
        // get the platform rectangle data
        Rectangle platRec = platform.getRec();

        // if there is a collision, 
        if (CheckCollisionRecs(playerRec, platRec)) {
            // assign a bunch of variables
            // the coordinates of all the player edges
            float playerBottom = playerRec.y + playerRec.height;
            float playerTop = playerRec.y;
            float playerLeft = playerRec.x;
            float playerRight = playerRec.x + playerRec.width;
                    
            // the coordinates of all the platform edges
            float platBottom = platRec.y + platRec.height;
            float platTop = platRec.y;
            float platLeft = platRec.x;
            float platRight = platRec.x + platRec.width;

            // the coordinates of the player's edges from the previous frame
            float prevBottom = prevY + height;
            float prevTop = prevY;
            float prevLeft = prevX;
            float prevRight = prevX + width;

            // Coming from above
            if ((prevBottom <= platTop) && (playerBottom > platTop)) {
                yPos = platTop - height;
                velocityY = 0;
                onGround = true;
            }
            // Coming from below
            else if (prevTop >= platBottom && playerTop < platBottom) {
                yPos = platBottom;
                velocityY = 0;
            }
            // Coming from left
            else if (prevRight <= platLeft && playerRight > platLeft) {
                xPos = platLeft - width;
            }
            // Coming from right
            else if (prevLeft >= platRight && playerLeft < platRight) {
                xPos = platRight;
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
    float deltaTime = GetFrameTime();
    // store previous position
    float prevX = xPos;
    float prevY = yPos;

    // check for jumping
    Jump();

    // do gravity
    if (hasGravity) {
        Gravity(deltaTime);
    }

    // check for moving
    Move(deltaTime);

    // check for platform collision
    platformCollision(platforms, prevX, prevY);

    // check for item collision
    itemCollision(items);
}

void Player::Draw() {
    DrawRectangle((int)xPos, (int)yPos, width, height, color);
}

Rectangle Player::getRec() const {
    return Rectangle{xPos, yPos, (float)width, (float)height};
}
