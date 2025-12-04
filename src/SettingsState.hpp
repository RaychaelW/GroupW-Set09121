#ifndef THE_QUEST_SETTINGSSTATE_HPP
#define THE_QUEST_SETTINGSSTATE_HPP

#include "State.hpp"
#include "StateManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SettingsState : public State {
public:
    SettingsState(StateManager& manager);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void updateHighlight();
    void applySettings();
    void loadSettings();

    StateManager& manager;

    // Background elements
    sf::Image backgroundImage;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Image gridImage;
    sf::Texture gridTexture;
    sf::Sprite gridSprite;

    // UI elements
    sf::Font font;
    sf::Text title;

    std::vector<sf::Text> options;
    std::vector<std::string> optionValues;

    int selectedIndex = 0;

    // Settings variables
    int masterVolume = 100;
    bool musicOn = true;
    bool sfxOn = true;
    bool fullscreen = false;
};

#endif // THE_QUEST_SETTINGSSTATE_HPP