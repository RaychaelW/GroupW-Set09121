#pragma once
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "StateManager.hpp"

class MainMenuState : public State {
public:
    MainMenuState(StateManager& manager);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    StateManager& manager;

    sf::Font font;
    sf::Text titleText;
    sf::Text startText;
};