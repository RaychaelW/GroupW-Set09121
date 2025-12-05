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

    // Handle input from the keyboard
    void handleInput();

    // Update player position and physics
    void update(float dt);

    // Draw player sprite on screen
    void render(sf::RenderWindow& window);

    // Collision bounds
    sf::Sprite& getSprite() { return sprite; }

    // Health system
    int getHealth() const { return health; }
    void takeDamage(); // reduces health by one
    bool isDead() const { return health <= 0; }

    // Coin system
    void collectCoin();
    int getCoins() const { return coins; }

private:
    InputManager input;
    sf::Sprite sprite;
    sf::Texture texture;

    // Movement
    sf::Vector2f velocity;
    float speed = 200.f;
    float jumpForce = 350.f;
    bool isGrounded = false;
    float gravity = 981.f;

    // Stats
    int health = 3;
    int coins = 0;
};

#endif //THE_QUEST_PLAYER_HPP