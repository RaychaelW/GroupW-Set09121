//
// Created by prais on 21/11/2025.
//

#ifndef THE_QUEST_PLAYER_HPP
#define THE_QUEST_PLAYER_HPP


#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.hpp"

class Player {
public:
    Player();

    void handleInput();
    void update(float dt);
    void render(sf::RenderWindow& window);

    sf::Sprite& getSprite() {
        return sprite;
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
    }

private:
    InputManager input;
    sf::Sprite sprite;
    sf::Texture texture;

    //movement
    sf::Vector2f velocity;
    float speed = 200.f;
    float jumpForce = 350.f;
    float isGrounded = false;
    float gravity = 981.f;
};


#endif //THE_QUEST_PLAYER_HPP