#include "MainMenuState.hpp"
#include "GameState.hpp"
//#include "KingdomSelectionState"
//#include "SettingsState"
#include <iostream>

#include "KingdomSelectionState.hpp"
#include "SettingsState.hpp"
#include "HowToPlayState.hpp"

MainMenuState::MainMenuState(StateManager& manager)
    : manager(manager){

    font.loadFromFile("resources/fonts/MedievalSharp-Regular.ttf");

    backgroundTexture.loadFromFile("resources/tilesets/Sprites/Backgrounds/bg.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(1.f, 1.f);

    // Title
    title.setFont(font);
    title.setString("THE QUEST");
    title.setCharacterSize(68);
    title.setStyle(sf::Text::Bold);
    title.setPosition(450, 80);
    title.setOutlineThickness(3);
    title.setOutlineColor(sf::Color::Black);

    // Play
    playText.setFont(font);
    playText.setString("Play");
    playText.setCharacterSize(40);
    playText.setPosition(500, 250);
    playText.setOutlineThickness(2);
    playText.setOutlineColor(sf::Color::Black);

    // Settings
    settingsText.setFont(font);
    settingsText.setString("Settings");
    settingsText.setCharacterSize(40);
    settingsText.setPosition(500, 330);
    settingsText.setOutlineThickness(2);
    settingsText.setOutlineColor(sf::Color::Black);

    // How To Play
    howToPlayText.setFont(font);
    howToPlayText.setString("How to Play");
    howToPlayText.setCharacterSize(40);
    howToPlayText.setPosition(500, 410);
    howToPlayText.setOutlineThickness(2);
    howToPlayText.setOutlineColor(sf::Color::Black);

    // Quit
    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(40);
    quitText.setPosition(500, 490);
    quitText.setOutlineThickness(2);
    quitText.setOutlineColor(sf::Color::Black);

    options = { &playText, &settingsText, &howToPlayText, &quitText };

    updateSelection();

    // Play main menu background music
    ResourceManager::getInstance().playMusic("resources/sounds/MainMenu.wav", true, 40.0f);
}

void MainMenuState::handleInput(sf::RenderWindow& window) {
    sf::Event event{};
    while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {

            // ESC does nothing here
            if (event.key.code == sf::Keyboard::Escape) {}

            if (event.key.code == sf::Keyboard::Up) {
                selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
                updateSelection();
                ResourceManager::getInstance().playSound("resources/sounds/Jump.wav", 30.0f);
            }

            if (event.key.code == sf::Keyboard::Down) {
                selectedIndex = (selectedIndex + 1) % options.size();
                updateSelection();
                ResourceManager::getInstance().playSound("resources/sounds/Jump.wav", 30.0f);
            }

            if (event.key.code == sf::Keyboard::Enter) {

                if (selectedIndex == 0) {
                    ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 60.0f);
                    manager.push(std::make_unique<KingdomSelectionState>(manager));
                }
                else if (selectedIndex == 1) {
                    ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 60.0f);
                    manager.push(std::make_unique<SettingsState>(manager));
                }
                else if (selectedIndex == 2) {
                    ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 60.0f);
                    manager.push(std::make_unique<SettingsState>(manager));
                }
                else if (selectedIndex == 3) {
                    ResourceManager::getInstance().playSound("resources/sounds/Jump.wav", 50.0f);
                    window.close();
                }
            }
        }
    }
}

void MainMenuState::update(float dt) {}

void MainMenuState::render(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(title);
    window.draw(playText);
    window.draw(settingsText);
    window.draw(howToPlayText);
    window.draw(quitText);
}

void MainMenuState::updateSelection() {
    for (auto* t : options)
        t->setFillColor(sf::Color::White);

    options[selectedIndex]->setFillColor(sf::Color::Yellow);
}
