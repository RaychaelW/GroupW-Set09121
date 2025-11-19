//
// Created by prais on 18/11/2025.
//

#ifndef THE_QUEST_MAINMENUSTATE_H
#define THE_QUEST_MAINMENUSTATE_H


#pragma once
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
    sf::Text title;
};



#endif //THE_QUEST_MAINMENUSTATE_H