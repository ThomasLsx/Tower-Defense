#include "entity.h"

/**
 * @class Castle
 * @brief Représente le château dans le jeu, hérite de Entity.
 */
class Castle : public Entity {
private:
    /**
     * @brief Points de vie actuels du château.
     */
    int HP;

    /**
     * @brief Points de vie maximum du château.
     */
    int maxHP;

public:
    /**
     * @brief Constructeur du château.
     * @param x Position X.
     * @param y Position Y.
     * @param hp Points de vie initiaux.
     * @param maxhp Points de vie maximum.
     * @param texture Texture SFML à utiliser.
     */
    Castle(int x, int y, int hp, int maxhp, const sf::Texture& texture);

    /**
     * @brief Constructeur par défaut.
     */
    Castle() = default;

    /**
     * @brief Destructeur.
     */
    ~Castle() override = default;

    /**
     * @brief Obtient les points de vie actuels.
     * @return Points de vie actuels.
     */
    int getHP() const;

    /**
     * @brief Obtient les points de vie maximum.
     * @return Points de vie maximum.
     */
    int getMaxHP() const;

    /**
     * @brief Définit les points de vie actuels.
     * @param hp Nouvelle valeur des points de vie.
     */
    void setHP(int hp);

    /**
     * @brief Définit les points de vie maximum.
     * @param maxHp Nouvelle valeur des points de vie maximum.
     */
    void setMaxHP(int maxHp);
};