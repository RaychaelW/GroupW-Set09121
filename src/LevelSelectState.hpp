#ifndef LEVELSELECTSTATE_HPP
#define LEVELSELECTSTATE_HPP

#include "State.hpp"
#include "StateManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class LevelSelectState : public State {
public:
    explicit LevelSelectState(StateManager& manager, int kingdomId = 1);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    StateManager& manager;
    sf::Font font;

    // Background
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // Title
    sf::Text titleText;

    // Level options
    sf::Text level1Text;
    sf::Text level2Text;
    sf::Text level3Text;
    sf::Text backText;

    std::vector<sf::Text*> options;
    int selectedIndex = 0;

    // Store which kingdom we're in
    int currentKingdomId;

    void updateSelection();
};

#endif // LEVELSELECTSTATE_HPP