#include "Obstacle.hpp"
#include <iostream>

Obstacle::Obstacle(float x, float y, const std::string& textureFile, float width, float height) {
    if (!textureFile.empty() && texture.loadFromFile(textureFile)) {
        sprite.setTexture(texture);

        float scaleX = width / sprite.getLocalBounds().width;
        float scaleY = height / sprite.getLocalBounds().height;
        sprite.setScale(scaleX, scaleY);
    } else {
        sprite.setTextureRect(sf::IntRect(0, 0, (int)width, (int)height));
        sprite.setColor(sf::Color::Red);
    }

    sprite.setPosition(x, y);
}

void Obstacle::update(float dt) {
    // NO MOVEMENT — platformer style obstacles
}

void Obstacle::render(sf::RenderWindow& window) {
    window.draw(sprite);
}