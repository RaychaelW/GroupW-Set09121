//
// Created by prais on 04/12/2025.
//

#ifndef THE_QUEST_GAMEOVERSTATE_HPP
#define THE_QUEST_GAMEOVERSTATE_HPP


#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "StateManager.hpp"

class GameOverState : public State {
public:
    GameOverState(StateManager& manager, sf::RenderWindow& window);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    StateManager& manager;
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text text;
};


#endif //THE_QUEST_GAMEOVERSTATE_HPP
