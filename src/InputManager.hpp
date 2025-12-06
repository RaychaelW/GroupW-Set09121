//
// Created by prais on 21/11/2025.
//

#ifndef THE_QUEST_INPUTMANAGER_HPP
#define THE_QUEST_INPUTMANAGER_HPP


#include <SFML/Window.hpp>
#include <unordered_map>
#include "SFML/Graphics.hpp"

class InputManager {
public:
    void update(); // call once per frame

    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool isKeyPressedOnce(sf::Keyboard::Key key);

    bool isMouseButtonPressed(sf::Mouse::Button button) const;
    sf::Vector2i getMousePosition(const sf::RenderWindow &window) const;

private:
    std::unordered_map<sf::Keyboard::Key, bool> previousKeyState;
};


#endif //THE_QUEST_INPUTMANAGER_HPP