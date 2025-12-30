#include "GameManager.hpp"

int GameManager::getScore() {
    return score;
}

void GameManager::updateScore(int addToScore) {
    score += addToScore;
}
