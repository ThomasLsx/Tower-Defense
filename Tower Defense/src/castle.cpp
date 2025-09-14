#include "castle.h"

Castle::Castle(int x, int y, int hp, int maxhp, const sf::Texture& texture)
	:Entity(x, y, texture), HP(hp), maxHP(maxhp)
{
}

int Castle::getHP() const
{
	return HP;
}

int Castle::getMaxHP() const
{
	return maxHP;
}

void Castle::setHP(int hp) 
{ 
	HP = hp; 
}

void Castle::setMaxHP(int maxHp) 
{ 
	maxHP = maxHp; 
}