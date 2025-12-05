//
// Created by prais on 18/11/2025.
//

#ifndef THE_QUEST_GAMESTATE_HPP
#define THE_QUEST_GAMESTATE_HPP


#pragma once
#include "Player.hpp"
#include "InputManager.hpp"
#include "State.hpp"
#include "StateManager.hpp"

class GameState : public State
{
public:
    GameState(StateManager& manager);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    StateManager& manager;
    InputManager input;
    Player player;

    // HUD
    sf::Sprite heartSprite;
    sf::Texture heartTexture;

    sf::Sprite coinSprite;
    sf::Texture coinTexture;

    sf::Font font;
    sf::Text coinText;
    sf::Clock clock;  // Used to measure time between heart damage presses

    int coinCount = 0;
};


#endif //THE_QUEST_GAMESTATE_HPP