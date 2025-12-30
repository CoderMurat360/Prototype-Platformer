#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>

using std::string;

class GlobalSettings {
    public:
        float screenWidth;
        float screenHeight;
        string windowTitle;
        float fps;

        GlobalSettings(float _screenWidth, float _screenHeight, string _windowTitle, float _fps) {
            screenWidth = _screenWidth;
            screenHeight = _screenHeight;
            windowTitle = _windowTitle;
            fps = _fps;
        }
};
GlobalSettings gs = GlobalSettings(800, 450, "Prototype Platformer", 60);

class GameManager {
    private:
        int score = 0;
    public:
        int getScore() {
            return score;
        }
        void updateScore(int addToScore) {
            score += addToScore;          
        }

};
GameManager gm;

class Platform {
    public:
        float xPos, yPos;
        int width, height;
        Color color;

        Platform(float x, float y, int w, int h, Color c) {
            xPos = x;
            yPos = y;
            width = w;
            height = h;
            color = c;
        }

        void Draw() {
            DrawRectangle((int) xPos, (int) yPos, width, height, color);
        }

        Rectangle getRec() const {
            return Rectangle{xPos, yPos, (float) width, (float) height};
        }
};

class Item {
    public:
        Vector2 center;
        int radius;
        bool collected = false;
        Color color;

        Item(Vector2 v, int r, Color c) {
            center = v;
            radius = r;
            color = c;
        }
        
        void Draw() {
            if (collected == false) {
                DrawCircleV(center, radius, color);
            }
        }
};

class Player {
    private:
        void Move() {
            if (IsKeyDown(KEY_A)) {
                xPos -= speed;
            }
            else if (IsKeyDown(KEY_D)) {
                xPos += speed;
            }
        }

        void Jump() {
            if (IsKeyDown(KEY_SPACE) && onGround) {
                velocity = -10.0f;
                onGround = false;
            }
        }

        void Gravity() {
            velocity += gravity;
            yPos += velocity;
        }
    
        void platformCollision(const std::vector<Platform>& platforms, float prevX, float prevY) {
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
                        velocity = 0;
                        onGround = true;
                    }
                    // Coming from below
                    else if (prevTop >= platBottom && playerTop < platBottom) {
                        yPos = platBottom;
                        velocity = 0;
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

        void itemCollision(std::vector<Item>& items) {
            for (auto& i: items) {
                if (i.collected == false) {
                    if (CheckCollisionCircleRec(i.center, i.radius, getRec())) {
                        i.collected = true;
                        gm.updateScore(1);
                    }
                }
            }
        }
    
    public:
        float xPos, yPos;
        int width, height;
        float speed = 5.0, velocity = 0.0, gravity = 0.4;
        bool onGround = false, hasGravity = true;
        Color color;

        Player(float x, float y, int _width, int _height, Color _color) {
            xPos = x;
            yPos = y;
            width = _width;
            height = _height;
            color = _color;
        }

        void Update(const std::vector<Platform>& platforms, std::vector<Item>& items) {
            // store previous position
            float prevX = xPos;
            float prevY = yPos;

            // check for jumping
            Jump();

            // do gravity
            if (hasGravity) {
                Gravity();
            }

            // check for moving
            Move();

            // check for platform collision
            platformCollision(platforms, prevX, prevY);

            // check for item collision
            itemCollision(items);
        }

        void Draw() {
            DrawRectangle((int) xPos, (int) yPos, width, height, color);
        }

        Rectangle getRec() const {
            return Rectangle{xPos, yPos, (float) width, (float) height};
        }
};


int main() {
    // initialize window and set fps
    InitWindow(gs.screenWidth, gs.screenHeight, gs.windowTitle.c_str());
    SetTargetFPS(gs.fps);

    // make player
    Player player = Player(gs.screenWidth/2, gs.screenHeight/2, 50, 50, RED);

    // make platforms
    std::vector<Platform> platforms;
    Platform platform1 = Platform(0, 400, 800, 50, BLUE);
    Platform platform2 = Platform(200, 300, 150, 20, BLUE);
    Platform platform3 = Platform(450, 250, 120, 20, BLUE);
    platforms.push_back(platform1);
    platforms.push_back(platform2);
    platforms.push_back(platform3);

    // Add invisible walls as platforms
    std::vector<Platform> boundaries;
    boundaries.push_back(Platform(-1, 0, 1, gs.screenHeight, BLACK));          // left wall
    boundaries.push_back(Platform(gs.screenWidth, 0, 1, gs.screenHeight, BLACK)); // right wall
    boundaries.push_back(Platform(0, -1, gs.screenWidth, 1, BLACK));            // ceiling

    // merge together platforms and boundaries
    std::vector<Platform> allPlatforms = platforms;
    allPlatforms.insert(allPlatforms.end(), boundaries.begin(), boundaries.end());

    //make items
    std::vector<Item> items;
    Item item1 = Item({gs.screenWidth/3, platform1.yPos-20}, 20, YELLOW);
    Item item2 = Item({platform2.xPos + platform2.width/2, platform2.yPos - 20}, 20, YELLOW);
    Item item3 = Item({platform3.xPos + platform3.width/2, platform3.yPos - 20}, 20, YELLOW);
    items.push_back(item1);
    items.push_back(item2);
    items.push_back(item3);


    while (!WindowShouldClose()) {
        BeginDrawing();
            // update
            player.Update(allPlatforms, items);

            // drawing
            ClearBackground(RAYWHITE);
            player.Draw();

            for (auto& platform : platforms) {
                platform.Draw();
            }
            for (auto& item : items) {
                item.Draw();
            }

            // make score text
            int gameScore = gm.getScore();
            string scoreText = "Score: " + std::to_string(gameScore);
            DrawText(scoreText.c_str(), gs.screenWidth/2, 20, 20, DARKGRAY);

            DrawText("Hello World! Press Space to jump and A and D to move left and right!", 10, gs.screenHeight-20, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
