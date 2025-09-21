#include "tower.h"

Tower::Tower(int x, int y, int damage, float txCritique, int prCritique, float portee, float cooldown, const sf::Texture& texture)
	: Entity(x, y, texture), Damage(damage), TxCritique(txCritique), prCritique(prCritique), Portee(portee), cooldown(cooldown)
{
}

int Tower::getDamage() const { return Damage; }
void Tower::setDamage(int damage) { Damage = damage; }

float Tower::getTxCritique() const { return TxCritique; }
void Tower::setTxCritique(float txCritique) { TxCritique = txCritique; }

int Tower::getPrCritique() const { return prCritique; }
void Tower::setPrCritique(int prCritique) { prCritique = prCritique; }

float Tower::getPortee() const { return Portee; }
void Tower::setPortee(float portee) { Portee = portee; }

float Tower::getCooldown() const { return cooldown; }
void Tower::setCooldown(float cooldown) { cooldown = cooldown; }

float Tower::getAttackSpeed() const { return attackSpeed; }
void Tower::setAttackSpeed(float attackSpeed) { attackSpeed = attackSpeed; }