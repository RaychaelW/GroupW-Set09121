#pragma once
#include <SFML/Graphics.hpp>
#include "StateManager.hpp"


class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    StateManager stateManager;
};