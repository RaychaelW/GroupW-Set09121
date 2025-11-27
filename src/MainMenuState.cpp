#include "MainMenuState.hpp"
#include "GameState.hpp"
#include <iostream>

MainMenuState::MainMenuState(StateManager& manager)
    : manager(manager)
{
    font.loadFromFile("resources/fonts/arial.ttf");

    titleText.setFont(font);
    titleText.setString("THE QUEST");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(200, 150);

    startText.setFont(font);
    startText.setString("Press ENTER to start");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(220, 300);
}

void MainMenuState::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        manager.pushState(std::make_unique<GameState>(manager));
    }
}

void MainMenuState::update(float dt) {}

void MainMenuState::render(sf::RenderWindow& window) {
    window.draw(titleText);
    window.draw(startText);
}