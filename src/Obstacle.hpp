#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Obstacle {
public:
    Obstacle(float x, float y, const std::string& textureFile, float width = 50.f, float height = 50.f);

    void update(float dt, float scrollSpeed);
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    bool isOffScreen() const { return sprite.getPosition().x + sprite.getGlobalBounds().width < 0.f; }

private:
    sf::Sprite sprite;
    sf::Texture texture;
};
