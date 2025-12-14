#include "minion.h"

#include <cmath>
#include <iostream>

namespace
{
    float length(const sf::Vector2f& v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    sf::Vector2f normalize(const sf::Vector2f& v)
    {
        float l = length(v);
        if (l != 0)
        {
            return v / l;
        }
        return v;
    }
}

Minion::Minion(int id, TileMap* map, Castle* castle, unsigned int health, float speed, unsigned int reward, sf::Vector2f pos, float rotation, sf::Color color)
    : Entity(id), map(map), castle(castle), health(health),
    copper(10), silver(10), gold(10),
    speed(speed), currentTargetIndex(0), maxHealth(health), specialStateTimer(0.0f)
{
}

void Minion::move()
{
    targetPath.clear();
    currentTargetIndex = 0;
}

void Minion::update(float dt)
{
    if (health > 0 && getIsAlive())
    {
        if (map->hasMapChanged())
        {
            move();
            map->setMapChanged(false);
        }
        followPath(dt);
    }
}

void Minion::followPath(float dt)
{
    if (!map)
    {
        return;
    }

    if (targetPath.empty())
    {
        sf::Vector2u myGridPos = map->getCurentTile(getPosition());
        Position currentPosStruct = { static_cast<int>(myGridPos.x), static_cast<int>(myGridPos.y) };

        auto nextPosOpt = map->getNextDirection(currentPosStruct);

        if (nextPosOpt.has_value())
        {
            Position nextP = nextPosOpt.value();

            sf::Vector2u nextGridPos(nextP.x, nextP.y);
            sf::Vector2f worldTarget = map->Tile2Position(nextGridPos);

            targetPath.clear();
            targetPath.push_back(worldTarget);
        }
        else
        {
            if (myGridPos == map->getCastleTile())
            {
                if (castle)
                {
                    castle->takeDamage(10);
                }
                this->onDestroy();
            }

            auto rescuePosOpt = map->getNearestAccessibleTile(currentPosStruct);

            if (rescuePosOpt.has_value()) {
                Position rescuePos = rescuePosOpt.value();
                sf::Vector2u rescueGridPos(rescuePos.x, rescuePos.y);

                targetPath.clear();
                targetPath.push_back(map->Tile2Position(rescueGridPos));
            }
            else {
                // std::cout << "[DEBUG] : Le minion" << id << "est vraiment bloqué" << std::endl;
                return;
            }
        }
    }

    if (!targetPath.empty())
    {
        sf::Vector2f target = targetPath[0];
        sf::Vector2f direction = target - getPosition();
        float dist = length(direction);
        float moveStep = speed * dt;

        if (dist <= moveStep)
        {
            setPosition(target);
            targetPath.clear();
        }
        else
        {
            setPosition(getPosition() + normalize(direction) * moveStep);
        }
    }
}

void Minion::onDestroy()
{
    if (_isAlive)
    {
        Entity::setIsAlive(false);
    }
}

void Minion::takeDamage(int amount)
{
    if (amount <= 0 || health == 0)
    {
        return;
    }

    if ((unsigned int)amount >= health)
    {
        health = 0;

        if (castle)
        {
            castle->addResource(copper, silver, gold);
        }

        this->onDestroy();
    }
    else
    {
        health -= amount;
    }
}

void Minion::draw(sf::RenderWindow& window)
{
    if (getIsAlive())
    {
        // Barre de vie (Fond)
        healthBarBack.setSize(sf::Vector2f(35.f, 6.f));
        healthBarBack.setFillColor(sf::Color(50, 50, 50, 200));
        healthBarBack.setOutlineColor(sf::Color::Black);
        healthBarBack.setOutlineThickness(1.f);
        healthBarBack.setOrigin(sf::Vector2f(17.5f, 15.f + 10.0f));
        healthBarBack.setPosition(this->getPosition());
        window.draw(healthBarBack);

        // Barre de vie (Remplissage)
        healthBar.setFillColor(sf::Color::Red);
        healthBar.setOrigin(sf::Vector2f(17.5f, 15.f + 10.0f));
        healthBar.setPosition(this->getPosition());

        float healthPercent = static_cast<float>(health) / static_cast<float>(maxHealth);
        if (healthPercent < 0)
        {
            healthPercent = 0;
        }
        healthBar.setSize(sf::Vector2f(35.f * healthPercent, 6.f));

        window.draw(healthBar);

        Entity::draw(window);
    }
}