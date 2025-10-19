#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    unsigned int money;
    unsigned int lives;
    unsigned int score;

public:
    Player(int startMoney = 500, int startLives = 20);

	void addMoney(int amount) { money += amount; }
    bool spendMoney(unsigned int amount) {
        if (amount > money) return false;
        money -= amount;
        return true;
	}

	void addScore(int points) { score += points; }

    int getMoney() const { return money; }
	int getLives() const { return lives; }
	int getScore() const { return score; }


    void loseLife() { if (lives > 0) lives--; }
};

#endif // PLAYER_H
