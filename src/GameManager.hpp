#pragma once

class GameManager {
    private:
        int score = 0;

    public:
        int getScore();
        void updateScore(int addToScore);
};
