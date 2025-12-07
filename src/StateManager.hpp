//
// Created by prais on 18/11/2025.
//

#ifndef THE_QUEST_STATEMANAGER_H
#define THE_QUEST_STATEMANAGER_H

#pragma once
#include <stack>
#include <memory>
#include <SFML/Graphics.hpp>
#include "State.hpp"

class StateManager {
private:
    std::stack<std::unique_ptr<State>> states;

public:
    void push(std::unique_ptr<State> state);
    void pop();
    void replace(std::unique_ptr<State> state);
    State* getCurrent();
    bool empty() const { return states.empty(); }

    void update(float dt);
    void render(sf::RenderWindow& window);
    void handleInput(sf::RenderWindow& window);
};

#endif //THE_QUEST_STATEMANAGER_H