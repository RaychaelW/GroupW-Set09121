#ifndef KINGDOMSELECTIONSTATE_HPP
#define KINGDOMSELECTIONSTATE_HPP

#include "State.hpp"
#include "StateManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class KingdomSelectionState : public State {
public:
    explicit KingdomSelectionState(StateManager& manager);

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

    // Kingdom options
    sf::Text kingdom1Text;
    sf::Text kingdom2Text;
    sf::Text backText;

    std::vector<sf::Text*> options;
    int selectedIndex = 0;

    void updateSelection();
};

#endif // KINGDOMSELECTIONSTATE_HPP