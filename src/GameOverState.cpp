#include "GameOverState.hpp"
#include "MainMenuState.hpp"
#include "GameState.hpp"
#include <iostream>


GameOverState::GameOverState(StateManager& manager, sf::RenderWindow& window)
: manager(manager), window(window)
{
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "GameOverState: Failed to load font\n";
    }
    text.setFont(font);
    text.setString("Game Over\nPress R to Retry\nPress M for Menu");
    text.setCharacterSize(28);
    text.setFillColor(sf::Color::White);
    text.setPosition(100.f, 100.f);
}

void GameOverState::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed) {
            // player can retry level
            if (event.key.code == sf::Keyboard::R) {
                manager.push(std::make_unique<GameState>(manager)); //start level 1 again
            }
            //player can return to menu
            else if (event.key.code == sf::Keyboard::M) {
                manager.push(std::make_unique<MainMenuState>(manager));
                return;
            }
        }
    }
}

void GameOverState::update(float dt) {

}

void GameOverState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    window.draw(text);
    window.setView(window.getDefaultView());
}