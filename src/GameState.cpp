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

        input.update(); //use Input manager
        player.handleInput(); // let the player read input(WASD)

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
        {
            manager.pop();   // Return to previous state (menu)
        }
    }
}

void GameState::update(float dt) {
    input.update();
    player.update(dt);

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
    player.render(window);
}
