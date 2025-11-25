#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player() {
    shape.setRadius(25.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(100.f, 400.f);
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        shape.move(0.f, -speed * 0.016f); // assuming ~60 FPS
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        shape.move(0.f, speed * 0.016f);
}

void Player::update(float dt) {
    // You can add bounds checking here
    if (shape.getPosition().y < 0)
        shape.setPosition(shape.getPosition().x, 0);
    if (shape.getPosition().y > 550)
        shape.setPosition(shape.getPosition().x, 550);
}

void Player::render(sf::RenderWindow& window) {
    window.draw(shape);
}
