#include "Game.hpp"
#include "MainMenuState.hpp"
#include <iostream>

Game::Game() : window(sf::VideoMode(800, 600), "The Quest") {
    window.setFramerateLimit(60);

    // Start with Main Menu
    stateManager.push(std::make_unique<MainMenuState>(stateManager));
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // CRITICAL: Use StateManager's handleInput, not getCurrent()->handleInput
        stateManager.handleInput(window);

        // Check if window should close
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // CRITICAL: Use StateManager's update and render methods
        stateManager.update(dt);

        window.clear();
        stateManager.render(window);
        window.display();

        // If state stack becomes empty, we should either:
        // 1. Push a new MainMenuState, OR
        // 2. Close the game (if that's what you want)
        if (stateManager.empty()) {
            std::cout << "State stack empty - closing game" << std::endl;
            window.close();
        }
    }
}