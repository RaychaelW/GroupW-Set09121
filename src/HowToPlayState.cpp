#include "HowToPlayState.hpp"
#include "ResourceManager.hpp"
#include "MainMenuState.hpp"
#include "StateManager.hpp"

HowToPlayState::HowToPlayState(StateManager& manager)
    : manager(manager) {  // REMOVED: State(manager),

    // Load font
    if (!font.loadFromFile("resources/fonts/Vipnagorgialla Rg.otf")) {
        // Handle error
    }

    // Load same background as main menu
    backgroundTexture.loadFromFile("resources/tilesets/Sprites/Backgrounds/bg.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(1.f, 1.f);

    // Title - BLACK TEXT
    title.setFont(font);
    title.setString("HOW TO PLAY");
    title.setCharacterSize(48);
    title.setStyle(sf::Text::Bold);
    title.setPosition(400, 50);
    title.setFillColor(sf::Color::Black);
    title.setOutlineThickness(3);
    title.setOutlineColor(sf::Color::White);

    // Instructions
    std::string instructionLines[] = {
        "CONTROLS:",
        "A / D - Move Left / Right",
        "SPACE - Jump",
        "E - Enemy Attack",
        "TAB - Toggle Map View",
        "P - Pause",
        "ESC - Return to Menu"
    };

    for (int i = 0; i < 6; ++i) {
        instructions[i].setFont(font);
        instructions[i].setString(instructionLines[i]);
        instructions[i].setCharacterSize(30);
        instructions[i].setPosition(300, 150 + i * 50);
        instructions[i].setFillColor(sf::Color::Black);
        instructions[i].setOutlineThickness(1);
        instructions[i].setOutlineColor(sf::Color::White);
    }

    // Back text
    backText.setFont(font);
    backText.setString("Press ESC to return to Main Menu");
    backText.setCharacterSize(25);
    backText.setPosition(350, 500);
    backText.setFillColor(sf::Color::Black);
    backText.setOutlineThickness(1);
    backText.setOutlineColor(sf::Color::White);
}

void HowToPlayState::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 50.0f);
                // Use pop() like ESC in KingdomSelectionState
                manager.push(std::make_unique<MainMenuState>(manager));
            }
        }
    }
}

void HowToPlayState::update(float dt) {
    // Nothing to update
}

void HowToPlayState::render(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(title);

    for (int i = 0; i < 6; ++i) {
        window.draw(instructions[i]);
    }

    window.draw(backText);
}