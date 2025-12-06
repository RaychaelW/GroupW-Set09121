#include "Kingdom1LevelState.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include "KingdomSelectionState.hpp"
#include "ResourceManager.hpp"  // ADDED: For sound

Kingdom1LevelState::Kingdom1LevelState(StateManager& manager)
    : manager(manager)
{
    font.loadFromFile("resources/fonts/MedievalSharp-Regular.ttf");

    // Create Shadow Caverns background
    backgroundImage.create(800, 600);

    for (int y = 0; y < 600; ++y) {
        for (int x = 0; x < 800; ++x) {
            // Base dark purple/blue cavern color
            float depth = (y / 600.0f) * 0.7f + 0.3f;

            // Create cavern-like patterns
            float noise1 = sin(x * 0.01f + y * 0.015f) * 0.3f;
            float noise2 = sin(x * 0.02f + y * 0.03f) * 0.2f;
            float noise3 = sin(x * 0.05f) * sin(y * 0.05f) * 0.1f;
            float pattern = noise1 + noise2 + noise3;

            int r = static_cast<int>(40 * depth + pattern * 20);
            int g = static_cast<int>(20 * depth + pattern * 10);
            int b = static_cast<int>(60 * depth + pattern * 30);

            // Add occasional "crystal" glints
            if ((x * 73 + y * 97) % 100 < 2) {
                r += 50;
                g += 30;
                b += 80;
            }

            r = std::max(10, std::min(r, 80));
            g = std::max(5, std::min(g, 40));
            b = std::max(20, std::min(b, 100));

            backgroundImage.setPixel(x, y, sf::Color(r, g, b));
        }
    }
    backgroundTexture.loadFromImage(backgroundImage);
    backgroundSprite.setTexture(backgroundTexture);

    // Add "stalactites" and "stalagmites" effect
    overlayImage.create(800, 600, sf::Color::Transparent);
    for (int x = 0; x < 800; ++x) {
        float stalactite = sin(x * 0.05f) * 30.0f + 40.0f;
        for (int y = 0; y < stalactite; ++y) {
            int alpha = static_cast<int>(255 * (1.0f - y / stalactite));
            overlayImage.setPixel(x, y, sf::Color(20, 10, 30, alpha));
        }

        float stalagmite = sin(x * 0.05f + 3.14f) * 30.0f + 40.0f;
        for (int y = 600 - static_cast<int>(stalagmite); y < 600; ++y) {
            int alpha = static_cast<int>(255 * ((y - (600 - stalagmite)) / stalagmite));
            overlayImage.setPixel(x, y, sf::Color(20, 10, 30, alpha));
        }
    }
    overlayTexture.loadFromImage(overlayImage);
    overlaySprite.setTexture(overlayTexture);

    // Title for the level
    title.setFont(font);
    title.setString("SHADOW CAVERNS");
    title.setCharacterSize(32);
    title.setPosition(150, 20);
    title.setFillColor(sf::Color(180, 160, 220));
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color(40, 20, 60));

    // Hint text
    hintText.setFont(font);
    hintText.setString("ESC to return to Kingdom Selection");
    hintText.setCharacterSize(15);
    hintText.setPosition(200, 560);
    hintText.setFillColor(sf::Color(180, 160, 220, 180));

    // ADDED: Play kingdom-specific background music
    ResourceManager::getInstance().playMusic("resources/sounds/GameBg.wav", true, 35.0f);
}

void Kingdom1LevelState::handleInput(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                // ADDED: Sound for returning to kingdom selection
                ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 50.0f);
                manager.push(std::make_unique<KingdomSelectionState>(manager));
            }
        }
    }
}

void Kingdom1LevelState::update(float dt)
{
    // Update level logic here
    // Note: In your actual gameplay, you might want to add:
    // - Level completion sound when reaching the end
    // - Enemy sounds
    // - Special power-up sounds

    // Example for level completion (you'll need to implement the actual logic):
    /*
    if (levelCompleted) {
        ResourceManager::getInstance().playSound("resources/sounds/KingdomCompleted.wav", 100.0f);
        // Stop background music and play victory music
        ResourceManager::getInstance().stopMusic();
    }
    */
}

void Kingdom1LevelState::render(sf::RenderWindow& window)
{
    // Draw background layers
    window.draw(backgroundSprite);
    window.draw(overlaySprite);

    // Draw level title
    window.draw(title);

    // Draw hint
    window.draw(hintText);
}