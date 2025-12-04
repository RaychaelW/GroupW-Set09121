#ifndef THE_QUEST_STATEMANAGER_H
#define THE_QUEST_STATEMANAGER_H

#pragma once
#include <stack>
#include <memory>
#include "State.hpp"

class StateManager {
private:
    std::stack<std::unique_ptr<State>> states;

public:
    void push(std::unique_ptr<State> state);
    void pop();
    State* getCurrent();

    // Add these essential methods:
    void handleInput(sf::RenderWindow& window);
    void update(float dt);
    void render(sf::RenderWindow& window);

    bool empty() const { return states.empty(); }
};

#endif //THE_QUEST_STATEMANAGER_H