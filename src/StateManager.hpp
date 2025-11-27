#pragma once
#include <memory>
#include <vector>
#include "State.hpp"

class StateManager {
public:
    void pushState(std::unique_ptr<State> state);
    void popState();
    State* getActiveState();

private:
    std::vector<std::unique_ptr<State>> states;
};