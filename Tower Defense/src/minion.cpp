#include "minion.h"

Minion::Minion(int x, int y, float damage, float speed, sf::Vector2f direction, const sf::Texture& texture)
	:Entity(x, y, texture), damage(damage), speed(speed), direction(direction)
{
}


float Minion::getDamage(void)
{
	return damage;
}
void Minion::setDamage(float damage)
{
	this->damage = damage;
}

sf::Vector2f Minion::getDirection(void)
{
	return direction;
}
void Minion::setDirection(sf::Vector2f direction)
{
	this->direction = direction;
}

float Minion::getSpeed(void)
{
	return speed;
}
void Minion::setSpeed(float speed)
{
	this->speed = speed;
}