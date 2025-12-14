#pragma once

#include <map>
#include <optional>
#include <set>
#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

/**
 * @brief Représente les coordonnées d'une tuile sur la grille.
 */
struct Position
{
    /** Coordonnée X de la tuile. */
    int x;
    /** Coordonnée Y de la tuile. */
    int y;

    // Opérateurs pour la comparaison (nécessaire pour std::map/std::set)
    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) const
    {
        return !(*this == other);
    }

    /**
     * @brief Opérateur de comparaison strict.
     * @return true si cette position est strictement inférieure à 'other'.
     */
    bool operator<(const Position& other) const
    {
        if (x != other.x)
        {
            return x < other.x;
        }

        return y < other.y;
    }
};

/**
 * @brief Implémente la logique de Flow Field basée sur Dijkstra Inversé.
 * * Permet de générer un champ de flux indiquant la direction vers une cible (Château)
 * pour toutes les tuiles atteignables de la carte.
 */
class Pathfinding
{
public:
    Pathfinding() = default;
    Pathfinding(const std::vector<std::vector<int>>& grid);

    /**
     * @brief Génère le Flow Field à l'aide de l'algorithme de Dijkstra Inversé.
     * * Le Flow Field est rempli de vecteurs normalisés pointant vers la cible.
     * * @param targetPos La position de la cible (ex: le Château).
     */
    void generateFlowField(Position targetPos);

    /**
     * @brief Récupère le vecteur de direction pour un index de tuile donné.
     * * @param tileIndex L'index de la tuile dans la grille 1D.
     * @return Un std::pair optionnel (dx, dy) représentant la direction.
     */
    std::optional<std::pair<int, int>> getVector(unsigned int tileIndex) const;

    /**
     * @brief Récupère la position de la prochaine tuile à atteindre depuis la position courante.
     * * @param currentPos La position de départ.
     * @return La position optionnelle de la prochaine tuile.
     */
    std::optional<Position> getNextMove(Position currentPos) const;

    /**
     * @brief Vérifie si le chemin vers la cible est bloqué.
     * @return true si le chemin est inaccessible.
     */
    bool isPathBlocked() const
    {
        return m_isPathBlocked;
    }

    /**
     * @brief Cherche la case valide la plus proche (celle qui a un chemin vers la cible).
     * @param startPos La position de départ (la case bloquée).
     * @param maxRadius Le rayon de recherche maximum (pour éviter de scanner toute la carte).
     * @return La position de la case valide la plus proche, ou std::nullopt.
     */
    std::optional<Position> getNearestValidPosition(Position startPos, int maxRadius = 5) const;

private:
    /** Largeur de la grille. */
    int m_width = 0;
    /** Hauteur de la grille. */
    int m_height = 0;

    /** Grille de référence (Murs/Tours). */
    std::vector<std::vector<int>> m_grid;

    /** Contient les vecteurs de direction : <Index Tuile, Vecteur(dx, dy)>. */
    std::map<unsigned int, std::pair<int, int>> m_vectorMap;

    /** Map des distances de Dijkstra (accessible par index 1D). */
    std::vector<int> m_distanceMap;

    /** État indiquant si la cible est inaccessible. */
    bool m_isPathBlocked = false;

    /**
     * @brief Vérifie si les coordonnées (x, y) sont à l'intérieur de la grille.
     */
    bool isValid(int x, int y) const;

    /**
     * @brief Vérifie si la tuile aux coordonnées (x, y) est praticable.
     */
    bool isWalkable(int x, int y) const;

    /**
     * @brief Convertit les coordonnées 2D (x, y) en un index 1D.
     */
    unsigned int getIndex(int x, int y) const;
};