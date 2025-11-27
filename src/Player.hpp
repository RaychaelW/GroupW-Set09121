#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    void handleInput();   // <-- no dt!
    void update(float dt);
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }

private:
    sf::CircleShape shape;
    float speed = 300.f;
};