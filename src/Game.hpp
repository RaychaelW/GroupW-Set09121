#pragma once
#include <SFML/Graphics.hpp>
#include "StateManager.hpp"

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    sf::Clock clock;
    StateManager stateManager;

    void processInput();
    void update(float dt);
    void render();
};