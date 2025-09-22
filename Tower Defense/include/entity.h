#pragma once
#include <utility>
#include <SFML/Graphics/Sprite.hpp>

struct Vector2 {
    float x, y;
    Vector2(float x = 0, float y = 0) : x(x), y(y) {}
};

class Entity {
protected:
    int id;
    Vector2 position;
    float rotation;
	sf::Sprite sprite;

public:
    Entity(void);
    Entity(int id, Vector2 pos, float rotation = 0.0f);
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;

	Vector2 getPosition() const { return position; }
	void setPosition(const Vector2& p) { position = p; }

    int getID() const { return id; }

    virtual void onDestroy() = 0;
};