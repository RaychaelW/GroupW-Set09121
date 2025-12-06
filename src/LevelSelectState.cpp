#include "LevelSelectState.hpp"
#include "Kingdom1LevelState.hpp"
#include "Kingdom2LevelState.hpp"
#include "KingdomSelectionState.hpp"
#include <iostream>

#include "GameState.hpp"

LevelSelectState::LevelSelectState(StateManager& manager, int kingdomId)
    : manager(manager), currentKingdomId(kingdomId)
{
    // Load font
    font.loadFromFile("resources/fonts/MedievalSharp-Regular.ttf");

    // Use different background based on kingdom
    if (currentKingdomId == 1) {
        // Shadow Caverns - Purple background
        backgroundTexture.loadFromFile("resources/tilesets/Sprites/Backgrounds/K1Levels.png");
        // OR create purple background if image not available
    } else {
        // Forest Realm - Green background
        backgroundTexture.loadFromFile("resources/tilesets/Sprites/Backgrounds/K2Levels.png");
        // OR create green background if image not available
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(2.4f, 2.7f);

    // Title - Show which kingdom we're in
    titleText.setFont(font);
    if (currentKingdomId == 1) {
        titleText.setString("SHADOW CAVERNS - LEVELS");
    } else {
        titleText.setString("FOREST REALM - LEVELS");
    }
    titleText.setCharacterSize(45);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(350, 80);
    titleText.setOutlineThickness(3);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setFillColor(sf::Color::White);

    // Level 1
    level1Text.setFont(font);
    level1Text.setString("Level 1");
    level1Text.setCharacterSize(40);
    level1Text.setPosition(500, 200);
    level1Text.setOutlineThickness(2);
    level1Text.setOutlineColor(sf::Color::Black);

    // Level 2
    level2Text.setFont(font);
    level2Text.setString("Level 2");
    level2Text.setCharacterSize(40);
    level2Text.setPosition(500, 280);
    level2Text.setOutlineThickness(2);
    level2Text.setOutlineColor(sf::Color::Black);

    // Level 3
    level3Text.setFont(font);
    level3Text.setString("Level 3");
    level3Text.setCharacterSize(40);
    level3Text.setPosition(500, 360);
    level3Text.setOutlineThickness(2);
    level3Text.setOutlineColor(sf::Color::Black);

    // Back option
    backText.setFont(font);
    backText.setString("<< Back to Kingdoms");
    backText.setCharacterSize(40);
    backText.setPosition(500, 440);
    backText.setOutlineThickness(2);
    backText.setOutlineColor(sf::Color::Black);

    // Add to options vector
    options.push_back(&level1Text);
    options.push_back(&level2Text);
    options.push_back(&level3Text);
    options.push_back(&backText);

    selectedIndex = 0;
    updateSelection();
}

void LevelSelectState::handleInput(sf::RenderWindow& window) {
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
                if (selectedIndex == 0) { // Level 1
                    // Go to the correct kingdom level
                    if (currentKingdomId == 1) {
                        manager.push(std::make_unique<GameState>(manager));
                    } else {
                        manager.push(std::make_unique<Kingdom2LevelState>(manager));
                    }
                }
                else if (selectedIndex == 1) { // Level 2
                    // For now, go to same as level 1
                    if (currentKingdomId == 1) {
                        manager.push(std::make_unique<Kingdom1LevelState>(manager));
                    } else {
                        manager.push(std::make_unique<Kingdom2LevelState>(manager));
                    }
                }
                else if (selectedIndex == 2) { // Level 3
                    // For now, go to same as level 1
                    if (currentKingdomId == 1) {
                        manager.push(std::make_unique<Kingdom1LevelState>(manager));
                    } else {
                        manager.push(std::make_unique<Kingdom2LevelState>(manager));
                    }
                }
                else if (selectedIndex == 3) { // Back
                    manager.push(std::make_unique<KingdomSelectionState>(manager));
                }
            }
        }
    }
}

void LevelSelectState::update(float dt) {
    // No update needed
}

void LevelSelectState::render(sf::RenderWindow& window) {
    /* Draw colored background based on kingdom
    if (currentKingdomId == 1) {
        // Shadow Caverns - Purple background
        window.clear(sf::Color(80, 40, 120)); // Dark purple
    } else {
        // Forest Realm - Green background
        window.clear(sf::Color(40, 80, 40)); // Dark green
    }
    */

    // If you want to keep the image background, uncomment:
    window.draw(backgroundSprite);

    window.draw(titleText);
    window.draw(level1Text);
    window.draw(level2Text);
    window.draw(level3Text);
    window.draw(backText);
}

void LevelSelectState::updateSelection() {
    for (auto* t : options)
        t->setFillColor(sf::Color::White);

    options[selectedIndex]->setFillColor(sf::Color::Yellow);
}