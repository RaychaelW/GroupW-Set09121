#include "GameOverState.hpp"
#include "MainMenuState.hpp"
#include "GameState.hpp"
#include <iostream>

#include "Kingdom1LevelState.hpp"
#include "Kingdom2LevelState.hpp"


GameOverState::GameOverState(StateManager& manager, KingdomID kingdom, LevelID level, int levelNumber)
    : manager(manager), window(window), kingdom(kingdom), level(level), levelNumber(levelNumber) {
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "GameOverState: Failed to load font\n";
    }
    // Game Over title
    titleText.setFont(font);
    titleText.setString("GAME OVER");
    titleText.setCharacterSize(90);
    titleText.setFillColor(sf::Color::Red);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(350.f, 200.f);

    // Instructions
    instructionText.setFont(font);
    instructionText.setString("Press R to Restart or M for Menu");
    instructionText.setCharacterSize(32);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(380.f, 400.f);
}

void GameOverState::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed) {
            // player can retry level
            if (event.key.code == sf::Keyboard::R) {
                if (kingdom == KingdomID::Kingdom1) {
                    manager.push(std::make_unique<Kingdom1LevelState>(manager, kingdom, level, levelNumber));
                } else if (kingdom == KingdomID::Kingdom2) {
                    manager.push(std::make_unique<Kingdom2LevelState>(manager, kingdom, level, levelNumber));
                }
                }
            }
            //player can return to menu
            else if (event.key.code == sf::Keyboard::M) {
                manager.push(std::make_unique<MainMenuState>(manager));
                return;
            }
        }
    }


void GameOverState::update(float dt) {

}

void GameOverState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    window.draw(titleText);
    window.draw(instructionText);
    window.setView(window.getDefaultView());
}