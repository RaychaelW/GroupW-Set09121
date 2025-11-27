#include "StateManager.hpp"

void StateManager::pushState(std::unique_ptr<State> state) {
    states.push_back(std::move(state));
}

void StateManager::popState() {
    if (!states.empty())
        states.pop_back();
}

State* StateManager::getActiveState() {
    if (states.empty()) return nullptr;
    return states.back().get();
}