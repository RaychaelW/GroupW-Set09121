//
// Created by prais on 18/11/2025.
//

#include "StateManager.hpp"

// Add a new state to the stack
void StateManager::push(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

// Remove the top state
void StateManager::pop() {
    if (!states.empty())
        states.pop();
}

// Replace the top state safely (used for switching screens)
void StateManager::replace(std::unique_ptr<State> state) {
    if (!states.empty())
        states.pop();
    states.push(std::move(state));
}

// Return the current active state
State* StateManager::getCurrent() {
    if (states.empty()) return nullptr;
    return states.top().get();
}

// Update only the active state
void StateManager::update(float dt) {
    if (!states.empty())
        states.top()->update(dt);
}

// Render only the active state
void StateManager::render(sf::RenderWindow& window) {
    if (!states.empty())
        states.top()->render(window);
}

// Handle input for the active state only
void StateManager::handleInput(sf::RenderWindow& window) {
    if (!states.empty())
        states.top()->handleInput(window);
}