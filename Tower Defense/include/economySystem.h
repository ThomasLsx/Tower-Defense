#ifndef ECONOMYSYSTEM_H
#define ECONOMYSYSTEM_H

class Player;
class Minion;

class EconomySystem {
private:
    Player* player;

public:
    EconomySystem(Player* p);
    ~EconomySystem();

    void rewardForKill(const Minion& minion);
    bool purchase(int cost);
    void refund(int amount);
};

#endif // ECONOMYSYSTEM_H
