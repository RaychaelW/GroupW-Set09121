//
// Created by prais on 21/11/2025.
//

#include "InputManager.hpp"

void InputManager::update() {
    // Store current key states for "pressed once" detection
    for (auto &pair : previousKeyState) {
        pair.second = sf::Keyboard::isKeyPressed(pair.first);
    }
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key);
}

bool InputManager::isKeyPressedOnce(sf::Keyboard::Key key) {
    bool currentlyPressed = sf::Keyboard::isKeyPressed(key);

    if (previousKeyState.find(key) == previousKeyState.end())
        previousKeyState[key] = false;

    bool pressedOnce = currentlyPressed && !previousKeyState[key];
    previousKeyState[key] = currentlyPressed;

    return pressedOnce;
}

bool InputManager::isMouseButtonPressed(sf::Mouse::Button button) const {
    return sf::Mouse::isButtonPressed(button);
}

sf::Vector2i InputManager::getMousePosition(const sf::RenderWindow &window) const {
    return sf::Mouse::getPosition(window);
}
