#pragma once
#include "State.hpp"
#include <memory>
#include <stack>

class StateManager {
public:
    void push(std::unique_ptr<State> state);
    void pop();
    State* getCurrent();

private:
    std::stack<std::unique_ptr<State>> states;
};
