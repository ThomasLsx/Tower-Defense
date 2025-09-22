#include "castle.h"

Castle::Castle(int x, int y, int hp, int maxhp, const sf::Texture& texture)
	:Entity(x, y, texture), HP(hp), maxHP(maxhp)
{
}