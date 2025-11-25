#include "Obstacle.hpp"
#include <iostream>

Obstacle::Obstacle(float x, float y, const std::string& textureFile, float width, float height) {
    if (!textureFile.empty() && texture.loadFromFile(textureFile)) {
        sprite.setTexture(texture);

        // Scale sprite to desired size
        float scaleX = width / sprite.getLocalBounds().width;
        float scaleY = height / sprite.getLocalBounds().height;
        sprite.setScale(scaleX, scaleY);
    } else {
        // fallback as rectangle if texture fails
        sprite.setTextureRect(sf::IntRect(0, 0, (int)width, (int)height));
    }

    sprite.setPosition(x, y);
}

void Obstacle::update(float dt, float scrollSpeed) {
    sprite.move(-scrollSpeed * dt, 0.f);
}

void Obstacle::render(sf::RenderWindow& window) {
    window.draw(sprite);
}
