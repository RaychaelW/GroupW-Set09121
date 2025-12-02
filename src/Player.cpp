//
// Created by prais on 21/11/2025.
//

#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include <iostream>


Player::Player() {
    texture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Spritesheets/spritesheet-characters-default.png");
    sprite.setTexture(*texture);
    sprite.setPosition(100.f, 300.f);
    velocity = sf::Vector2f(0.f, 0.f);

    int frameWidth = 100;
    int frameHeight = 100;

    //default beige character
    frames[CharacterColor::Beige][MoveType::Idle] = {
        {525, 924, frameWidth, frameHeight} // 1 idle frames
    };
    frames[CharacterColor::Beige][MoveType::Walk] = {
        {526, 539, frameWidth, frameHeight}, {525, 667, frameWidth, frameHeight} // 2 walk frames
    };
    frames[CharacterColor::Beige][MoveType::Jump] = {
        {527, 795, frameWidth, frameHeight} // 1 jump frame
    };
}

void Player::updateAnimation(float dt) {
    animationTimer += dt;
    if (animationTimer >= animationSpeed) {
        animationTimer = 0.f;
        currentFrame++;

        const auto& moveFrames = frames[currentColor][currentMove];
        if (!moveFrames.empty()) {
            currentFrame %= moveFrames.size();
            Frame f = moveFrames[currentFrame];
            sprite.setTextureRect(sf::IntRect(f.x, f.y, f.width, f.height));
        }

        sprite.setScale(facingRight ? 1.f : -1.f, 1.f);
        if (!facingRight)
            sprite.setOrigin(128, 0);
        else
            sprite.setOrigin(0, 0);
    }
}


void Player::handleInput() {
    // horizontal movement
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= speed;
        currentMove = MoveType::Walk;
        facingRight = false;
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += speed;
        currentMove = MoveType::Walk;
        facingRight = true;
    }
    else {
        currentMove = MoveType::Idle;
    }

    //jump
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        velocity.y = -jumpForce;
        isGrounded = false;
        currentMove = MoveType::Jump;
        return;

    }

    if (!isGrounded) {
        currentMove = MoveType::Jump;
    }
}


void Player::update(float dt, const sf::RenderWindow& window) {
    //apply gravity
    velocity.y += gravity * dt;

    sprite.move(velocity * dt);


    //simple ground check
    if (sprite.getPosition().y >= 544.f) {
        sprite.setPosition(sprite.getPosition().x, 544.f);
        velocity.y = 0.f;
        isGrounded = true;
    }

    const sf::FloatRect bounds = sprite.getGlobalBounds(); //get player bounds
    // Top collision
    if (bounds.top < 0) {
        velocity.y = 0.f;
    }
    //left wall collision
    if (bounds.left < 0) {
        velocity.x = 0.f;
    }

    /* doesn't work for now
    if (bounds.left + bounds.width > window.getSize().x) {
        velocity.x = 0.f;
    }
    */


    updateAnimation(dt); //animate

}



void Player::render(sf::RenderWindow& window) {

    window.draw(sprite);
}
