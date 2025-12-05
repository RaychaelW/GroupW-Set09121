#include "Game.hpp"
#include "MainMenuState.hpp"


Game::Game() : window(sf::VideoMode(1280, 720), "The Quest")
{
    stateManager.push(std::make_unique<MainMenuState>(stateManager));
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Process input
        if (auto* state = stateManager.getCurrent()) {
            state->handleInput(window);
            state->update(dt);
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Render
        window.clear();
        if (auto* state = stateManager.getCurrent())
            state->render(window);
        window.display();
    }
}