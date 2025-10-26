#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>
#include <limits>
#include <optional> // Pour un retour propre en C++ moderne

// --- Définitions Boost et Types de base ---

// Définir un tag pour la propriété de position
namespace boost {
    enum vertex_position_t { vertex_position };
    BOOST_INSTALL_PROPERTY(vertex, position);
}

// Structure pour les coordonnées
struct Position {
    int x, y;
    // Opérateur de comparaison pour les tests (optionnel mais utile)
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// Définitions du graphe
typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    boost::property<boost::vertex_position_t, Position>,
    boost::property<boost::edge_weight_t, int>
> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::property_map<Graph, boost::vertex_position_t>::type PositionMap;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

struct ManhattanHeuristic : public boost::astar_heuristic<Graph, int> {
    ManhattanHeuristic(Vertex goal, PositionMap positions)
        : m_goal(goal), m_positions(positions) {
    }

    int operator()(Vertex u) {
        Position p_u = boost::get(m_positions, u);
        Position p_goal = boost::get(m_positions, m_goal);
        return abs(p_u.x - p_goal.x) + abs(p_u.y - p_goal.y);
    }
private:
    Vertex m_goal;
    PositionMap m_positions;
};


// --- Classe Pathfinding ---

class Pathfinding {
public:
    /**
     * @brief Construit le graphe de pathfinding à partir d'une grille.
     * @param grid Matrice 2D où 0 = passable, 1 (ou autre) = mur.
     */
    Pathfinding(const std::vector<std::vector<int>>& grid);

    /**
     * @brief Trouve le chemin le plus court entre deux positions.
     * @param startPos Position de départ {x, y}.
     * @param goalPos Position d'arrivée {x, y}.
     * @return Un std::vector de Positions représentant le chemin,
     * ou std::nullopt si aucun chemin n'est trouvé.
     */
    std::optional<std::vector<Position>> findPath(Position startPos, Position goalPos);

private:
    /**
     * @brief Obtient le descripteur de sommet pour une position donnée.
     * @return Le Vertex correspondant, ou std::nullopt si la position
     * est hors limites ou est un mur.
     */
    std::optional<Vertex> getVertex(Position pos) const;

    Graph m_graph;
    PositionMap m_positionMap;
    std::vector<std::vector<Vertex>> m_gridToVertex; // Mappe les (x,y) aux sommets
    int m_height;
    int m_width;
};