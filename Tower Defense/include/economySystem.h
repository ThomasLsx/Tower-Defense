// economySystem.h

#pragma once
#include "Game.h"

class Game;

class EconomySystem
{
public:
	EconomySystem(Game* game);
	~EconomySystem();

private:
	int copper;
	int silver;
	int gold;

	Game* game;

public:
	inline int getCopper() const { return copper; }
	inline int getSilver() const { return silver; }
	inline int getGold() const { return gold; }
	inline void addCopper(int amount) { copper += amount; }
	inline void addSilver(int amount) { silver += amount; }
	inline void addGold(int amount) { gold += amount; }
	
	bool spendCopper(int amount);
	bool spendSilver(int amount);
	bool spendGold(int amount);
};

