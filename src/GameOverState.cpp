#include "GameOverState.hpp"
#include "GameState.hpp"
#include "MainMenuState.hpp"
#include <iostream>

GameOverState::GameOverState(StateManager& manager)
    : manager(manager)
{
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "Failed to load font in GameOverState" << std::endl;
    }

    // Game Over title
    titleText.setFont(font);
    titleText.setString("GAME OVER");
    titleText.setCharacterSize(90);
    titleText.setFillColor(sf::Color::Red);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(180.f, 200.f);

    // Instructions
    instructionText.setFont(font);
    instructionText.setString("Press R to Restart or M for Menu");
    instructionText.setCharacterSize(32);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(160.f, 400.f);
}

void GameOverState::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {

            // Restart game — properly replace GameOver with new GameState
            if (event.key.code == sf::Keyboard::R) {

                manager.push(std::make_unique<GameState>(manager));
                return; // prevent multiple actions
            }

            // Go to main menu — same logic
            else if (event.key.code == sf::Keyboard::M) {
                manager.push(std::make_unique<MainMenuState>(manager));
                return;
            }
        }
    }
}

void GameOverState::update(float dt) {
    // No logic needed for now
}

void GameOverState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    window.draw(titleText);
    window.draw(instructionText);
    // Don't call window.display() — main loop handles that
}