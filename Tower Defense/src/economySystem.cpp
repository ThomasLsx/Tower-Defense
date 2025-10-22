
#include "economySystem.h"
#include "player.h"
#include "minion.h"

EconomySystem::EconomySystem(Player* p) : player(p) {}

EconomySystem::~EconomySystem() {}

void EconomySystem::rewardForKill(const Minion& minion) {
    // TODO: ajouter la valeur de récompense du minion
    player->addMoney(10);
    player->addScore(100);
}

bool EconomySystem::purchase(int cost) {
    return player->spendMoney(cost);
}

void EconomySystem::refund(int amount) {
    player->addMoney(amount);
}
