#include "Kingdom2LevelState.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

Kingdom2LevelState::Kingdom2LevelState(StateManager& manager)
    : manager(manager)
{
    font.loadFromFile("resources/fonts/MedievalSharp-Regular.ttf");

    // Create Forest Realm background
    backgroundImage.create(800, 600);

    for (int y = 0; y < 600; ++y) {
        for (int x = 0; x < 800; ++x) {
            // Vertical gradient from sky to forest floor
            float gradient = y / 600.0f;

            int r, g, b;

            if (gradient < 0.3f) { // Sky
                float skyPos = gradient / 0.3f;
                r = static_cast<int>(100 + skyPos * 20);
                g = static_cast<int>(150 + skyPos * 30);
                b = static_cast<int>(200 + skyPos * 20);
            } else if (gradient < 0.6f) { // Tree canopy
                float canopyPos = (gradient - 0.3f) / 0.3f;
                r = static_cast<int>(30 + canopyPos * 40);
                g = static_cast<int>(80 + canopyPos * 70);
                b = static_cast<int>(40 + canopyPos * 30);
            } else { // Forest floor
                float floorPos = (gradient - 0.6f) / 0.4f;
                r = static_cast<int>(60 + floorPos * 20);
                g = static_cast<int>(120 + floorPos * 30);
                b = static_cast<int>(60 + floorPos * 10);
            }

            // Add foliage patterns
            float leafPattern = sin(x * 0.02f) * cos(y * 0.03f) * 0.4f;
            int tempR = r + static_cast<int>(leafPattern * 20);
            r = std::max(20, std::min(tempR, 180));

            int tempG = g + static_cast<int>(leafPattern * 30);
            g = std::max(50, std::min(tempG, 200));

            int tempB = b + static_cast<int>(leafPattern * 15);
            b = std::max(20, std::min(tempB, 120));

            // Add "light rays" effect
            float lightRay = sin((x + y * 0.5f) * 0.01f);
            if (lightRay > 0.9f && gradient > 0.2f && gradient < 0.5f) {
                r += 30;
                g += 40;
                b += 20;
            }

            backgroundImage.setPixel(x, y, sf::Color(r, g, b));
        }
    }
    backgroundTexture.loadFromImage(backgroundImage);
    backgroundSprite.setTexture(backgroundTexture);

    // Create tree overlay
    treeImage.create(800, 600, sf::Color::Transparent);
    for (int x = 0; x < 800; x += 80) {
        // Draw tree trunks
        for (int trunkX = x + 30; trunkX < x + 50; trunkX++) {
            for (int y = 400; y < 600; y++) {
                int trunkWidth = 20 - abs(trunkX - (x + 40));
                if (y < 400 + trunkWidth * 2) {
                    // Bark texture
                    int barkR = 80 + (x + y) % 20;
                    int barkG = 50 + (x + y) % 15;
                    int barkB = 30 + (x + y) % 10;
                    treeImage.setPixel(trunkX, y, sf::Color(barkR, barkG, barkB, 255));
                }
            }
        }

        // Draw foliage (tree tops)
        for (int fx = x; fx < x + 80; fx++) {
            for (int fy = 300; fy < 450; fy++) {
                float dist = sqrt(pow(fx - (x + 40), 2) + pow(fy - 380, 2));
                if (dist < 40) {
                    int alpha = static_cast<int>(255 * (1.0f - dist / 40.0f));
                    int green = 60 + static_cast<int>(100 * (1.0f - dist / 40.0f));
                    treeImage.setPixel(fx, fy, sf::Color(30, green, 20, alpha));
                }
            }
        }
    }
    treeTexture.loadFromImage(treeImage);
    treeSprite.setTexture(treeTexture);

    // Title
    title.setFont(font);
    title.setString("FOREST REALM");
    title.setCharacterSize(48);
    title.setPosition(150, 20);
    title.setFillColor(sf::Color(160, 220, 160));
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color(20, 40, 20));

    // Hint text
    hintText.setFont(font);
    hintText.setString("ESC to return to Kingdom Selection");
    hintText.setCharacterSize(20);
    hintText.setPosition(200, 560);
    hintText.setFillColor(sf::Color(160, 220, 160, 180));
}

void Kingdom2LevelState::handleInput(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                manager.pop();  // Return to kingdom selection
            }
        }
    }
}

void Kingdom2LevelState::update(float dt)
{
    // Update level logic here
}

void Kingdom2LevelState::render(sf::RenderWindow& window)
{
    // Draw background
    window.draw(backgroundSprite);
    window.draw(treeSprite);

    // Draw UI
    window.draw(title);
    window.draw(hintText);
}