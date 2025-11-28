// economySystem.cpp

#include "economySystem.h"
#include <iostream>

EconomySystem::EconomySystem(Game* game)
	: copper(100), silver(50), gold(10)
{
}

EconomySystem::~EconomySystem()
{
}

bool EconomySystem::spendCopper(int amount)
{
    if (copper >= amount) {
        copper -= amount;
        return true;
    }
	std::cout << "Not enough copper!" << std::endl;
    return false;
}

bool EconomySystem::spendSilver(int amount)
{
    if (silver >= amount) {
        silver -= amount;
        return true;
    }
    std::cout << "Not enough silver!" << std::endl;

    return false;
}

bool EconomySystem::spendGold(int amount)
{
    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    std::cout << "Not enough gold!" << std::endl;
    return false;
}
