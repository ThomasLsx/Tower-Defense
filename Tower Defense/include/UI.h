// UI.h
#pragma once

#include <memory>

#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Picture.hpp>

#include "Game.h"
#include "Window.h"

class Window;
class Game;
class WaveManager;

/**
 * @brief Classe gérant l'interface utilisateur (UI) du jeu via TGUI.
 */
class UI
{
public:
    /**
     * @brief Constructeur de la classe UI.
     * @param window Pointeur vers la fenêtre principale.
     * @param game Pointeur vers l'instance du jeu.
     */
    UI(Window* window, Game* game);

    /** @brief Instance principale de l'interface graphique TGUI. */
    tgui::Gui gui;

    /**
     * @brief Gère les événements d'entrée pour l'interface utilisateur.
     * @param event L'événement SFML à traiter.
     */
    void handleEvent(const sf::Event& event);

    /**
     * @brief Met à jour la disposition des éléments UI (redimensionnement).
     */
    void updateUILayout();

    /**
     * @brief Dessine l'interface utilisateur.
     */
    void draw();

    // UI Menu Modes

    /**
     * @brief Initialise les widgets du menu principal.
     */
    void initMenuUI();

    /**
     * @brief Affiche le menu principal et masque les autres interfaces.
     */
    void showMenuUI();

    // UI Play Modes

    /**
     * @brief Initialise les widgets de l'interface de jeu.
     */
    void initPlayUI();

    /**
     * @brief Met à jour les informations affichées dans l'interface de jeu (argent, vagues, etc.).
     */
    void updatePlayUI();

    /**
     * @brief Affiche l'interface de jeu et masque les autres.
     */
    void showPlayUI();

    // UI Editor Modes

    /**
     * @brief Initialise les widgets de l'éditeur de niveau.
     */
    void initEditorUI();

    /**
     * @brief Affiche l'interface de l'éditeur et masque les autres.
     */
    void showEditorUI();

    /**
     * @brief Récupère l'index du bouton de tourelle actuellement sélectionné.
     * @return L'index du bouton, ou -1 si aucun n'est sélectionné.
     */
    int getSelectedTurretButtonIndex() const;

    /**
     * @brief Désélectionne tous les boutons de tourelles.
     */
    void deselectTurretButtons();

private:
    /** @brief Groupe de widgets pour le menu principal. */
    tgui::Group::Ptr menuUI;

    /** @brief Groupe de widgets pour l'interface de jeu. */
    tgui::Group::Ptr playUI;

    /** @brief Groupe de widgets pour l'éditeur. */
    tgui::Group::Ptr editorUI;

    /** @brief État de pause du jeu. */
    bool isPaused = false;

    /** @brief Index du bouton de tourelle sélectionné (-1 si aucun). */
    int selectedTurretButtonIndex = -1;

    /** @brief Liste des boutons de sélection de tourelles. */
    std::vector<tgui::Button::Ptr> turretButtons;

    /** @brief Largeur précédente de la fenêtre (pour le redimensionnement). */
    float prevWindowWidth;

    /** @brief Hauteur précédente de la fenêtre (pour le redimensionnement). */
    float prevWindowHeight;

    /** @brief Pointeur vers l'instance de la fenêtre wrapper. */
    Window* window;

    /** @brief Pointeur vers l'instance logique du jeu. */
    Game* game;

    /**
     * @brief Centre horizontalement un widget à une position verticale donnée.
     * @param widget Le widget à centrer.
     * @param y La position verticale (Y).
     */
    void centerWidget(tgui::Widget::Ptr widget, float y);

    /** @brief Enumération pour l'alignement des boutons. */
    enum class ButtonAlign
    {
        Left,
        Center,
        Right
    };

    /**
     * @brief Place une liste de boutons verticalement.
     * @param buttons Vecteur contenant les boutons à placer.
     * @param startY Position Y de départ.
     * @param margin Marge latérale.
     * @param spacing Espace entre les boutons.
     * @param align Type d'alignement horizontal.
     */
    void placeButtonsStacked(const std::vector<tgui::Button::Ptr>& buttons, float startY, float margin, float spacing, ButtonAlign align);
};