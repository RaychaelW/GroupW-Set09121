#ifndef THE_QUEST_MAINMENUSTATE_HPP
#define THE_QUEST_MAINMENUSTATE_HPP

#include "State.hpp"
#include "StateManager.hpp"
#include <SFML/Graphics.hpp>

class MainMenuState : public State {
public:
    MainMenuState(StateManager& manager);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void updateSelection();

    StateManager& manager;

    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Text title;
    sf::Text playText;
    sf::Text settingsText;
    sf::Text quitText;

    std::vector<sf::Text*> options;
    int selectedIndex = 0;
};

#endif
