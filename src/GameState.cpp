#include "GameState.hpp"
#include <iostream>

/**
 Initializes all resources and HUD components for the game.
 */
GameState::GameState(StateManager& manager)
    : manager(manager)
{
    // Load textures for heart and coin icons
    if (!heartTexture.loadFromFile("resources/textures/heart.png"))
        std::cerr << "Failed to load heart texture" << std::endl;
    if (!coinTexture.loadFromFile("resources/textures/coin.png"))
        std::cerr << "Failed to load coin texture" << std::endl;

    heartSprite.setTexture(heartTexture);
    heartSprite.setScale(0.4f, 0.4f);
    coinSprite.setTexture(coinTexture);
    coinSprite.setScale(0.4f, 0.4f);

    // Load font for HUD text
    if (!font.loadFromFile("resources/fonts/arial.ttf"))
        std::cerr << "Failed to load font" << std::endl;

    coinText.setFont(font);
    coinText.setCharacterSize(22);
    coinText.setFillColor(sf::Color::White);
}

/**
 * @brief Handles input events such as closing window or key presses.
 */
void GameState::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    input.update();
    player.handleInput();

    // Test feature: Press 'H' to lose ONE heart
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
        static float lastPress = 0;
        float now = clock.getElapsedTime().asSeconds();
        if (now - lastPress > 0.5f) { // prevents spamming
            player.takeDamage();
            lastPress = now;
        }
    }
}

/**
 *  Updates the player and HUD elements.
 */
void GameState::update(float dt) {
    player.update(dt);
    coinText.setString("x " + std::to_string(player.getCoins()));
}

/**
 * Renders player and HUD (hearts + coins) on screen.
 */
void GameState::render(sf::RenderWindow& window) {
    player.render(window);

    // Draw hearts (fade individually as health decreases)
    for (int i = 0; i < 3; ++i) {
        heartSprite.setPosition(10.f + i * 50.f, 10.f);

        if (i < player.getHealth()) {
            heartSprite.setColor(sf::Color(255, 255, 255, 255)); // visible
        } else {
            heartSprite.setColor(sf::Color(255, 255, 255, 70)); // faded
        }

        window.draw(heartSprite);
    }

    // Draw coin HUD
    coinSprite.setPosition(180.f, 10.f);
    window.draw(coinSprite);

    coinText.setPosition(220.f, 12.f);
    window.draw(coinText);
}