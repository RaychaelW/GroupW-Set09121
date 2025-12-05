#include "Player.hpp"
#include <iostream>


Player::Player() {
    if (!texture.loadFromFile("resources/textures/player.png")) {
        std::cerr << "Failed to load player texture" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(100.f, 300.f);
    sprite.setScale(0.5f, 0.5f); // Adjust player size visually
    velocity = sf::Vector2f(0.f, 0.f);
}/**
  Handles keyboard inputs for player movement and jumping.
 */
void Player::handleInput() {
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        velocity.x -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        velocity.x += speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && isGrounded) {
        velocity.y = -jumpForce;
        isGrounded = false;
    }
}


void Player::update(float dt) {
    // Apply gravity
    velocity.y += gravity * dt;
    sprite.move(velocity * dt);

    // Ground collision (temporary floor level)
    if (sprite.getPosition().y >= 500.f) {
        sprite.setPosition(sprite.getPosition().x, 500.f);
        velocity.y = 0.f;
        isGrounded = true;
    }
}

/**
 * Renders player sprite on the window.
 */
void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

/**
 *  Reduces player's health by 1 if hit by an obstacle or event.
 */
void Player::takeDamage() {
    if (health > 0) {
        health--;
        std::cout << "Player took damage! Remaining health: " << health << std::endl;
    }
}

/**
 *  Increases coin count when a coin is collected.
 */
void Player::collectCoin() {
    coins++;
}