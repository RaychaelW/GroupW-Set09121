#include <SFML/Graphics.hpp>
#include "StateManager.hpp"
#include "GameState.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "The Quest");

    StateManager stateManager;
    stateManager.push(std::make_unique<GameState>(stateManager));

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        if (auto* state = stateManager.getCurrent()) {
            state->handleInput(window);
            state->update(dt);

            window.clear(sf::Color::Black);
            state->render(window);
            window.display();
        }
    }

    return 0;
}
