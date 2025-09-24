#pragma once
#include "entity.h"

class Castle : public Entity {
private:
    unsigned int health;
    unsigned int maxHealth;

public:
    Castle(int id, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), float rotation = 0.0f, sf::Color color = sf::Color::White, sf::IntRect textureRect = sf::IntRect(), const sf::Texture& texture = sf::Texture(), unsigned int maxHealth=100);
    
    void takeDamage(unsigned int amount);
    void repair(unsigned int amount);
    void update(float dt) override;
     
	unsigned int getHealth() const { return health; }
    unsigned int getMaxHealth() const { return maxHealth; }

    void onDestroy() override;
};
