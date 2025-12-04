#include "MainMenuState.hpp"
#include "GameState.hpp"
#include <iostream>

MainMenuState::MainMenuState(StateManager& manager)
    : manager(manager){
    font.loadFromFile("resources/fonts/arial.ttf");
    title.setFont(font);
    title.setString("The Quest\nPress ENTER to Play");
    title.setCharacterSize(25);
    title.setPosition(100, 100);
}

void MainMenuState::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Enter)
        {
            manager.push(std::make_unique<GameState>(manager));
        }
    }
}

void MainMenuState::update(float dt) {}

void MainMenuState::render(sf::RenderWindow& window) {
    window.draw(title);
}
