//
// Created by prais on 04/12/2025.
//

#ifndef THE_QUEST_GAMEOVERSTATE_HPP
#define THE_QUEST_GAMEOVERSTATE_HPP

#pragma once
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "StateManager.hpp"
#include "GameState.hpp"
#include "MainMenuState.hpp"


 //Displays the Game Over screen with restart and menu options.

class GameOverState : public State {
public:
    GameOverState(StateManager& manager);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    StateManager& manager;
    sf::Font font;
    sf::Text titleText;
    sf::Text instructionText;
};


#endif //THE_QUEST_GAMEOVERSTATE_HPP
