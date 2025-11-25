#pragma once
#include <SFML/Graphics.hpp>
#include "StateManager.hpp"

// Game objects
#include "Player.hpp"

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    StateManager stateManager;

    // ===== GAME OBJECTS =====
    Player player;
    Enemy enemy1;
    Enemy enemy2;
};
