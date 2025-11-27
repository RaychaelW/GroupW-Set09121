#include "Game.hpp"
#include "GameState.hpp"
#include "StateManager.hpp"

Game::Game() {
    window.create(sf::VideoMode(800, 600), "The Quest");
    window.setFramerateLimit(60);

    stateManager.pushState(std::make_unique<GameState>(stateManager));
}

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        processInput();
        update(dt);
        render();
    }
}

void Game::processInput() {
    if (stateManager.getActiveState())
        stateManager.getActiveState()->handleInput(window);
}

void Game::update(float dt) {
    if (stateManager.getActiveState())
        stateManager.getActiveState()->update(dt);
}

void Game::render() {
    window.clear();

    if (stateManager.getActiveState())
        stateManager.getActiveState()->render(window);

    window.display();
}