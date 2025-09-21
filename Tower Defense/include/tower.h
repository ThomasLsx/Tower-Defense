#pragma once

#include "entity.h"

/**
 * @class Tower
 * @brief Représente une tour dans le jeu, héritée de Entity.
 */
class Tower : public Entity {
private:
    /**
     * @brief Dégâts infligés par la tour.
     */
    int Damage;

    /**
     * @brief Taux de coup critique.
     */
    float TxCritique;

    /**
     * @brief Probabilité de coup critique.
     */
    int prCritique;

    /**
     * @brief Portée de la tour.
     */
    float Portee;

    /**
     * @brief Temps de recharge entre les attaques.
     */
    float cooldown;

    /**
     * @brief Vitesse d'attaque de la tour.
     */
    float attackSpeed;

public:
    /**
     * @brief Constructeur de la tour.
     * @param x Position X.
     * @param y Position Y.
     * @param damage Dégâts.
     * @param txCritique Taux de coup critique.
     * @param prCritique Probabilité de coup critique.
     * @param portee Portée.
     * @param cooldown Temps de recharge.
     * @param attackSpeed Vitesse d'attaque.
     * @param texture Texture SFML à utiliser.
     */
    Tower(int x, int y, int damage, float txCritique, int prCritique, float portee, float cooldown, float attackSpeed, const sf::Texture& texture);

    /**
     * @brief Constructeur par défaut.
     */
    Tower() = default;

    /**
     * @brief Destructeur.
     */
    ~Tower() override = default;

    /**
     * @brief Obtient les dégâts de la tour.
     * @return Dégâts.
     */
    int getDamage() const;

    /**
     * @brief Définit les dégâts de la tour.
     * @param damage Nouvelle valeur des dégâts.
     */
    void setDamage(int damage);

    /**
     * @brief Obtient le taux de coup critique.
     * @return Taux de coup critique.
     */
    float getTxCritique() const;

    /**
     * @brief Définit le taux de coup critique.
     * @param txCritique Nouvelle valeur du taux.
     */
    void setTxCritique(float txCritique);

    /**
     * @brief Obtient la probabilité de coup critique.
     * @return Probabilité de coup critique.
     */
    int getPrCritique() const;

    /**
     * @brief Définit la probabilité de coup critique.
     * @param prCritique Nouvelle valeur de la probabilité.
     */
    void setPrCritique(int prCritique);

    /**
     * @brief Obtient la portée de la tour.
     * @return Portée.
     */
    float getPortee() const;

    /**
     * @brief Définit la portée de la tour.
     * @param portee Nouvelle valeur de la portée.
     */
    void setPortee(float portee);

    /**
     * @brief Obtient le temps de recharge.
     * @return Temps de recharge.
     */
    float getCooldown() const;

    /**
     * @brief Définit le temps de recharge.
     * @param cooldown Nouvelle valeur du temps de recharge.
     */
    void setCooldown(float cooldown);

    /**
     * @brief Obtient la vitesse d'attaque.
     * @return Vitesse d'attaque.
     */
    float getAttackSpeed() const;

    /**
     * @brief Définit la vitesse d'attaque.
     * @param attackSpeed Nouvelle valeur de la vitesse d'attaque.
     */
    void setAttackSpeed(float attackSpeed);
};