
#ifndef THE_QUEST_HOWTOPLAYSTATE_HPP
#define THE_QUEST_HOWTOPLAYSTATE_HPP


#include "State.hpp"
#include <SFML/Graphics.hpp>

#include "StateManager.hpp"

class HowToPlayState : public State {
public:
    explicit HowToPlayState(StateManager& manager);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text title;
    sf::Text instructions[6];  // For multiple lines of instructions
    sf::Text backText;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    StateManager& manager;
};


#endif //THE_QUEST_HOWTOPLAYSTATE_HPP