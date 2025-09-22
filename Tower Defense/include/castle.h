#pragma once
#include "entity.h"

class Castle : public Entity {
private:
    int health;
    int maxHealth;

public:
    Castle(int id, Vector2 pos, int maxHealth);
    void takeDamage(int amount);
    void repair(int amount);
    void update(float dt) override;
     
	int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }

    void onDestroy() override;
};
