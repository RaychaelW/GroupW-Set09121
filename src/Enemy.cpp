#include "Enemy.hpp"
#include <iostream>

Enemy::Enemy(float x, float y) {
    if (!texture.loadFromFile("resources/textures/enemies/snail_walk.png")) {
        std::cerr << "Failed to load enemy.png!" << std::endl;
    }

    sprite.setTexture(texture);

    // Optional: scale down if image is too big
    sprite.setScale(0.5f, 0.5f);

    sprite.setPosition(x, y);
}

void Enemy::update(float dt) {
    // For now, enemy is stationary. Any AI can be added here later.
}

void Enemy::render(sf::RenderWindow& window) {
    window.draw(sprite);
}
