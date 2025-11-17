#pragma once
#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    void run();
private:
    sf::RenderWindow window;
};