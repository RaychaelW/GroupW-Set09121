#include "StateManager.hpp"
#include "State.hpp"
#include <SFML/Graphics.hpp>

void StateManager::push(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void StateManager::pop() {
    if (!states.empty())
        states.pop();
}

State* StateManager::getCurrent() {
    if (states.empty()) return nullptr;
    return states.top().get();
}

void StateManager::handleInput(sf::RenderWindow& window) {
    if (!states.empty()) {
        states.top()->handleInput(window);
    }
}

void StateManager::update(float dt) {
    if (!states.empty()) {
        states.top()->update(dt);
    }
}

void StateManager::render(sf::RenderWindow& window) {
    if (!states.empty()) {
        states.top()->render(window);
    }
}