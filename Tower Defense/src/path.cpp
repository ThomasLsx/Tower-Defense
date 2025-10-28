#include "path.h"
#include <boost/graph/graph_traits.hpp>
#include <iostream>

Pathfinding::Pathfinding(const std::vector<std::vector<int>>& grid)
    : m_height(grid.size()), m_width(0)
{
    if (m_height == 0) return;
    m_width = grid[0].size();
    if (m_width == 0) return;

    // Initialiser la map de position
    m_positionMap = boost::get(boost::vertex_position, m_graph);

    // Initialiser la grille de mapping avec un sommet "nul"
    Vertex null_v = boost::graph_traits<Graph>::null_vertex();
    m_gridToVertex.resize(m_height, std::vector<Vertex>(m_width, null_v));

    // --- 1. Ajouter les sommets ---
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if (grid[i][j] == 3 || grid[i][j] == 4 || grid[i][j] == 7) { // Cases accessibles
                Vertex v = boost::add_vertex(m_graph);
                m_gridToVertex[i][j] = v;
                m_positionMap[v] = { i, j }; // Assigner la position
            }
        }
    }

    // --- 2. Ajouter les arêtes (non orientées) ---
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if (grid[i][j] == 3) {
                Vertex current_v = m_gridToVertex[i][j];

                // Voisin de droite (j+1)
                if (j + 1 < m_width && (grid[i][j + 1] == 3 || grid[i][j + 1] == 4 || grid[i][j + 1] == 7)) {
                    Vertex neighbor_v = m_gridToVertex[i][j + 1];
                    if (neighbor_v != null_v && current_v != null_v) {
                        Edge e; bool inserted;
                        boost::tie(e, inserted) = boost::add_edge(current_v, neighbor_v, m_graph);
                        boost::put(boost::edge_weight_t(), m_graph, e, 1);
                    }
                }

                // Voisin de gauche (j-1)
                if (j - 1 >= 0 && (grid[i][j - 1] == 3 || grid[i][j - 1] == 4 || grid[i][j - 1] == 7)) {
                    Vertex neighbor_v = m_gridToVertex[i][j - 1];
                    if (neighbor_v != null_v && current_v != null_v) {
                        Edge e; bool inserted;
                        boost::tie(e, inserted) = boost::add_edge(current_v, neighbor_v, m_graph);
                        boost::put(boost::edge_weight_t(), m_graph, e, 1);
                    }
                }

                // Voisin du bas (i+1)
                if (i + 1 < m_height && (grid[i + 1][j] == 3 || grid[i + 1][j] == 4 || grid[i + 1][j] == 7)) {
                    Vertex neighbor_v = m_gridToVertex[i + 1][j];
                    if (neighbor_v != null_v && current_v != null_v) {
                        Edge e; bool inserted;
                        boost::tie(e, inserted) = boost::add_edge(current_v, neighbor_v, m_graph);
                        boost::put(boost::edge_weight_t(), m_graph, e, 1);
                    }
                }

                // Voisin du haut (i-1)
                if (i - 1 >= 0 && (grid[i - 1][j] == 3 || grid[i - 1][j] == 4 || grid[i - 1][j] == 7)) {
                    Vertex neighbor_v = m_gridToVertex[i - 1][j];
                    if (neighbor_v != null_v && current_v != null_v) {
                        Edge e; bool inserted;
                        boost::tie(e, inserted) = boost::add_edge(current_v, neighbor_v, m_graph);
                        boost::put(boost::edge_weight_t(), m_graph, e, 1);
                    }
                }
            }
        }
    }
}

std::optional<Vertex> Pathfinding::getVertex(Position pos) const {
    if (pos.x < 0 || pos.x >= m_height || pos.y < 0 || pos.y >= m_width) {
		std::cout << "Position hors limites: (" << pos.x << ", " << pos.y << ")" << std::endl;
        return std::nullopt; // Hors limites
    }

    Vertex v = m_gridToVertex[pos.x][pos.y];
    if (v == boost::graph_traits<Graph>::null_vertex()) {
		std::cout << "Position inaccessible (mur): (" << pos.x << ", " << pos.y << ")" << std::endl;
        return std::nullopt; // C'est un mur
    }

    return v;
}

std::optional<std::vector<Position>> Pathfinding::findPath(Position startPos, Position goalPos) {

    std::optional<Vertex> startOpt = getVertex(startPos);
    std::optional<Vertex> goalOpt = getVertex(goalPos);

    // Vérifier si le départ et l'arrivée sont valides (accessibles)
    if (!startOpt || !goalOpt) {
        return std::nullopt; // Départ ou arrivée sur un mur/hors grille
    }

    Vertex start = *startOpt;
    Vertex goal = *goalOpt;

    // Cartes pour A*
    std::vector<Vertex> predecessors(boost::num_vertices(m_graph));
    std::vector<int> distances(boost::num_vertices(m_graph));

    // Exécuter A*
    try {
        boost::astar_search(
            m_graph, start,
            ManhattanHeuristic(goal, m_positionMap),
            boost::visitor(boost::default_astar_visitor()).
            predecessor_map(&predecessors[0]).
            distance_map(&distances[0]).
            weight_map(boost::get(boost::edge_weight_t(), m_graph)).
            distance_compare(std::less<int>()).
            distance_combine(boost::closed_plus<int>()).
            distance_inf(std::numeric_limits<int>::max()).
            distance_zero(0)
        );
    }
    catch (...) {
        // TODO : Gérer une éventuelle exception de Boost 
        return std::nullopt;
    }

    // --- Reconstruire le chemin ---

    // Vérifier si un chemin a été trouvé
    if (distances[goal] == std::numeric_limits<int>::max()) {
        return std::nullopt; // Aucun chemin trouvé
    }

    std::vector<Position> path;
    for (Vertex v = goal; v != start; v = predecessors[v]) {
        path.push_back(m_positionMap[v]);
    }
    path.push_back(m_positionMap[start]); // Ajouter le point de départ

    std::reverse(path.begin(), path.end());

    return path;
}