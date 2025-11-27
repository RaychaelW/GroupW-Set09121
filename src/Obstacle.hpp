#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    Obstacle(float x, float y, const std::string& textureFile, float width, float height);

    void update(float dt); // NO scrollSpeed
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    bool isOffScreen() const { return false; } // never auto-delete for platformer

private:
    sf::Texture texture;
    sf::Sprite sprite;
};