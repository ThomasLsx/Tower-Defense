#include "projectile.h"

Projectile::Projectile(int x, int y, float damage, float speed, sf::Vector2f direction, const sf::Texture& texture)
	:Entity(x, y, texture), damage(damage), speed(speed), direction(direction)
{
}


float Projectile::getDamage(void) 
{ 
	return damage; 
}
void Projectile::setDamage(float damage) 
{ 
	this->damage = damage; 
}

sf::Vector2f Projectile::getDirection(void) 
{ 
	return direction; 
}
void Projectile::setDirection(sf::Vector2f direction) 
{ 
	this->direction = direction; 
}

float Projectile::getSpeed(void) 
{ 
	return speed; 
}
void Projectile::setSpeed(float speed) 
{ 
	this->speed = speed; 
}