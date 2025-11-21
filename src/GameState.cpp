//
// Created by prais on 18/11/2025.
//

#include "GameState.hpp"
#include <iostream>


GameState::GameState(StateManager& manager)
    : manager(manager)
{}

void GameState::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
        {
            manager.pop();   // Return to previous state (menu)
        }
    }
}

void GameState::update(float dt) {
    input.update();

    if (input.isKeyPressed(sf::Keyboard::A)) {
        //player.moveLeft(); commented out for now until player controls is implemented
    }

    if (input.isKeyPressed(sf::Keyboard::D)) {
        //player.moveRight();  commented out for now until player controls is implemented
    }

    if (input.isKeyPressedOnce(sf::Keyboard::Escape)) {
        // open pause menu
    }

}

void GameState::render(sf::RenderWindow& window) {
    // TEMP placeholder
    sf::CircleShape player(30);
    player.setFillColor(sf::Color::Green);
    player.setPosition(300, 300);
    window.draw(player);
}
