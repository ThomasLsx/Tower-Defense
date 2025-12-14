#pragma once

#include "Game.h"

class Game;

/**
 * @brief Système gérant l'économie du jeu (ressources monétaires).
 */
class EconomySystem
{
public:
    /**
     * @brief Constructeur du système économique.
     * @param game Pointeur vers l'instance principale du jeu.
     */
    EconomySystem(Game* game);

    /**
     * @brief Destructeur du système économique.
     */
    ~EconomySystem();

private:
    /** @brief Quantité de cuivre (ressource de base). */
    int copper;

    /** @brief Quantité d'argent (ressource intermédiaire). */
    int silver;

    /** @brief Quantité d'or (ressource rare). */
    int gold;

    /** @brief Pointeur vers l'instance du jeu (contexte). */
    Game* game;

public:
    /**
     * @brief Récupère la quantité de cuivre actuelle.
     * @return Le montant de cuivre.
     */
    inline int getCopper() const
    {
        return copper;
    }

    /**
     * @brief Récupère la quantité d'argent actuelle.
     * @return Le montant d'argent.
     */
    inline int getSilver() const
    {
        return silver;
    }

    /**
     * @brief Récupère la quantité d'or actuelle.
     * @return Le montant d'or.
     */
    inline int getGold() const
    {
        return gold;
    }

    /**
     * @brief Ajoute une quantité de cuivre.
     * @param amount Montant à ajouter.
     */
    inline void addCopper(int amount)
    {
        copper += amount;
    }

    /**
     * @brief Ajoute une quantité d'argent.
     * @param amount Montant à ajouter.
     */
    inline void addSilver(int amount)
    {
        silver += amount;
    }

    /**
     * @brief Ajoute une quantité d'or.
     * @param amount Montant à ajouter.
     */
    inline void addGold(int amount)
    {
        gold += amount;
    }

    /**
     * @brief Tente de dépenser du cuivre.
     * @param amount Montant à dépenser.
     * @return true si la transaction a réussi (fonds suffisants), false sinon.
     */
    bool spendCopper(int amount);

    /**
     * @brief Tente de dépenser de l'argent.
     * @param amount Montant à dépenser.
     * @return true si la transaction a réussi, false sinon.
     */
    bool spendSilver(int amount);

    /**
     * @brief Tente de dépenser de l'or.
     * @param amount Montant à dépenser.
     * @return true si la transaction a réussi, false sinon.
     */
    bool spendGold(int amount);
};