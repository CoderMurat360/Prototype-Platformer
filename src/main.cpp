#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>

// include all of the classes
#include "GameManager.hpp"
#include "Player.hpp"
#include "Platform.hpp"
#include "Item.hpp"

using std::string;

struct GameSettings {
    float screenWidth, screenHeight, fps;
    string windowTitle;
};
GameSettings SetSettings(float w, float h, float f, string wt) {
    GameSettings gs;
    gs.screenWidth = w;
    gs.screenHeight = h;
    gs.fps = f;
    gs.windowTitle = wt;
}

// create game manager object
GameManager gm;
// create game settings struct variable
GameSettings gs = SetSettings(800, 450, 60, "Prototype Platformer");

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
