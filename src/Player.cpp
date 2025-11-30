//
// Created by prais on 21/11/2025.
//

#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"


Player::Player() {
    texture.loadFromFile("resources/tilesets/Sprites/Characters/Default/character_pink_walk_a.png");
    sprite.setTexture(texture);
    sprite.setPosition(100.f, 300.f);
    velocity = sf::Vector2f(0.f, 0.f);
}

void Player::handleInput() {
    // horizontal movement
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += speed;
    }

    //jump
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        velocity.y = -jumpForce;
        isGrounded = false;
    }

}


void Player::update(float dt) {
    //apply gravity
    velocity.y += gravity * dt;

    sprite.move(velocity * dt);

    //simple ground check (stops at y=500 for now)
    if (sprite.getPosition().y >= 512.f) {
        sprite.setPosition(sprite.getPosition().x, 512.f);
        velocity.y = 0.f;
        isGrounded = true;
    }

}

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
}