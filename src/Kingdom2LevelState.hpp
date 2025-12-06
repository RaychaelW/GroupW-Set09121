
#ifndef THE_QUEST_KINGDOM2LEVELSTATE_HPP
#define THE_QUEST_KINGDOM2LEVELSTATE_HPP


#include "State.hpp"
#include "StateManager.hpp"
#include <SFML/Graphics.hpp>

class Kingdom2LevelState : public State {
public:
    Kingdom2LevelState(StateManager& manager);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    StateManager& manager;

    // Background elements
    sf::Image backgroundImage;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Image treeImage;
    sf::Texture treeTexture;
    sf::Sprite treeSprite;

    // UI elements
    sf::Font font;
    sf::Text title;
    sf::Text hintText;

    // Game objects (add these as needed)
    // Player player;
    // std::vector<Enemy> enemies;
    // std::vector<Platform> platforms;
    // etc.
};


#endif //THE_QUEST_KINGDOM2LEVELSTATE_HPP