#include "Kingdom1LevelState.hpp"
#include <iostream>

Kingdom1LevelState::Kingdom1LevelState(StateManager& manager)
    : manager(manager)
{
    font.loadFromFile("resources/fonts/MedievalSharp-Regular.ttf");

    // Create an image first, then load it into texture
    sf::Image backgroundImage;
    backgroundImage.create(800, 600);

    // Simple dark purple gradient
    for (int y = 0; y < 600; ++y) {
        for (int x = 0; x < 800; ++x) {
            // Simple dark purple gradient
            int r = 40 + (y / 10);
            int g = 20 + (y / 15);
            int b = 60 + (y / 8);

            // Clamp values
            if (r > 80) r = 80;
            if (g > 40) g = 40;
            if (b > 100) b = 100;

            backgroundImage.setPixel(x, y, sf::Color(r, g, b));
        }
    }

    // Load the image into texture
    backgroundTexture.loadFromImage(backgroundImage);
    backgroundSprite.setTexture(backgroundTexture);

    // Title
    title.setFont(font);
    title.setString("SHADOW CAVERNS - SELECT LEVEL");
    title.setCharacterSize(42);
    title.setStyle(sf::Text::Bold);
    title.setPosition(80, 40);
    title.setFillColor(sf::Color(180, 160, 220));
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color(40, 20, 60));

    // Level 1
    level1.setFont(font);
    level1.setString("Level 1: Dark Depths");
    level1.setCharacterSize(36);
    level1.setPosition(150, 180);
    level1.setOutlineThickness(1);
    level1.setOutlineColor(sf::Color(40, 20, 60));

    // Level 2
    level2.setFont(font);
    level2.setString("Level 2: Forgotten Gate");
    level2.setCharacterSize(36);
    level2.setPosition(150, 260);
    level2.setOutlineThickness(1);
    level2.setOutlineColor(sf::Color(40, 20, 60));

    // Level 3
    level3.setFont(font);
    level3.setString("Level 3: Cavern Heart");
    level3.setCharacterSize(36);
    level3.setPosition(150, 340);
    level3.setOutlineThickness(1);
    level3.setOutlineColor(sf::Color(40, 20, 60));

    // Back button
    backButton.setFont(font);
    backButton.setString("< BACK to Kingdom Selection");
    backButton.setCharacterSize(28);
    backButton.setPosition(150, 460);
    backButton.setFillColor(sf::Color(200, 200, 200));
    backButton.setOutlineThickness(1);
    backButton.setOutlineColor(sf::Color(50, 50, 50));

    // Hint text
    hintText.setFont(font);
    hintText.setString("Press ENTER to select level | ESC to go back");
    hintText.setCharacterSize(20);
    hintText.setPosition(150, 520);
    hintText.setFillColor(sf::Color(180, 160, 220, 180));

    options = { &level1, &level2, &level3 };
    selectedIndex = 0;
    updateHighlight();
}

void Kingdom1LevelState::handleInput(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                manager.pop();  // Return to kingdom selection
            }

            if (event.key.code == sf::Keyboard::Up) {
                selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
                updateHighlight();
            }

            if (event.key.code == sf::Keyboard::Down) {
                selectedIndex = (selectedIndex + 1) % options.size();
                updateHighlight();
            }

            if (event.key.code == sf::Keyboard::Enter) {
                std::cout << "Selected Shadow Caverns - Level " << (selectedIndex + 1) << std::endl;
                // TODO: Someone else will implement pushing to actual game level here
            }
        }
    }
}

void Kingdom1LevelState::update(float dt) {}

void Kingdom1LevelState::render(sf::RenderWindow& window)
{
    window.draw(backgroundSprite);
    window.draw(title);
    window.draw(level1);
    window.draw(level2);
    window.draw(level3);
    window.draw(backButton);
    window.draw(hintText);
}

void Kingdom1LevelState::updateHighlight()
{
    for (int i = 0; i < options.size(); i++) {
        if (i == selectedIndex) {
            options[i]->setFillColor(sf::Color(255, 215, 0)); // Gold for selection
        } else {
            options[i]->setFillColor(sf::Color(180, 160, 220)); // Normal purple
        }
    }
}