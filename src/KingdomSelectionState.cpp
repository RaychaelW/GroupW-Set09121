#include "KingdomSelectionState.hpp"
#include "LevelSelectState.hpp"
#include "MainMenuState.hpp"
#include <iostream>

KingdomSelectionState::KingdomSelectionState(StateManager& manager)
    : manager(manager)
{
    // Load font
    font.loadFromFile("resources/fonts/MedievalSharp-Regular.ttf");

    // Load background
    backgroundTexture.loadFromFile("resources/textures/KingdomB.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(1.3f, 1.3f);

    // Title
    titleText.setFont(font);
    titleText.setString("SELECT KINGDOM");
    titleText.setCharacterSize(68);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(150, 80);
    titleText.setOutlineThickness(3);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setFillColor(sf::Color::White);

    // Kingdom 1 option
    kingdom1Text.setFont(font);
    kingdom1Text.setString("Kingdom1: Shadow Caverns");
    kingdom1Text.setCharacterSize(40);
    kingdom1Text.setPosition(150, 200);
    kingdom1Text.setOutlineThickness(2);
    kingdom1Text.setOutlineColor(sf::Color::Black);

    // Kingdom 2 option
    kingdom2Text.setFont(font);
    kingdom2Text.setString("Kingdom2: Forest Realm");
    kingdom2Text.setCharacterSize(40);
    kingdom2Text.setPosition(150, 280);
    kingdom2Text.setOutlineThickness(2);
    kingdom2Text.setOutlineColor(sf::Color::Black);

    // Back option
    backText.setFont(font);
    backText.setString("<< Back");
    backText.setCharacterSize(40);
    backText.setPosition(150, 360);
    backText.setOutlineThickness(2);
    backText.setOutlineColor(sf::Color::Black);

    // Add to options vector
    options.push_back(&kingdom1Text);
    options.push_back(&kingdom2Text);
    options.push_back(&backText);

    selectedIndex = 0;
    updateSelection();
}

void KingdomSelectionState::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                manager.pop();
            }

            if (event.key.code == sf::Keyboard::Up) {
                selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
                updateSelection();
            }

            if (event.key.code == sf::Keyboard::Down) {
                selectedIndex = (selectedIndex + 1) % options.size();
                updateSelection();
            }

            if (event.key.code == sf::Keyboard::Enter) {
                if (selectedIndex == 0) { // Kingdom 1
                    // Pass kingdom ID 1 to LevelSelectState
                    manager.push(std::make_unique<LevelSelectState>(manager, 1));
                }
                else if (selectedIndex == 1) { // Kingdom 2
                    // Pass kingdom ID 2 to LevelSelectState
                    manager.push(std::make_unique<LevelSelectState>(manager, 2));
                }
                else if (selectedIndex == 2) { // Back
                    manager.pop();
                }
            }
        }
    }
}

void KingdomSelectionState::update(float dt) {
    // No update needed
}

void KingdomSelectionState::render(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(titleText);
    window.draw(kingdom1Text);
    window.draw(kingdom2Text);
    window.draw(backText);
}

void KingdomSelectionState::updateSelection() {
    for (auto* t : options)
        t->setFillColor(sf::Color::White);

    options[selectedIndex]->setFillColor(sf::Color::Yellow);
}