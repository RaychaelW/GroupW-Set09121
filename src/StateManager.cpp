#include "StateManager.hpp"

void StateManager::push(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void StateManager::pop() {
    if (!states.empty())
        states.pop();
}

State* StateManager::getCurrent() {
    if (states.empty())
        return nullptr;
    return states.top().get();
}
