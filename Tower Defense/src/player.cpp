#include "player.h"

Player::Player(int startMoney, int startLives)
    : money(startMoney), lives(startLives), score(0) {
}

void Player::addMoney(int amount) {
    money += amount;
}

bool Player::spendMoney(int amount) {
    if (money >= amount) {
        money -= amount;
        return true;
    }
    return false;
}

void Player::loseLife() {
    if (lives > 0) lives--;
}

void Player::addScore(int points) {
    score += points;
}

int Player::getMoney() const { return money; }
int Player::getLives() const { return lives; }
int Player::getScore() const { return score; }
