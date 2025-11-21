//
// Created by prais on 18/11/2025.
//

#ifndef THE_QUEST_STATE_H
#define THE_QUEST_STATE_H

//ensures all states share the same functions.

#pragma once
#include <SFML/Graphics.hpp>

class State {
public:
    virtual ~State() = default;

    virtual void handleInput(sf::RenderWindow& window) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};



#endif //THE_QUEST_STATE_H