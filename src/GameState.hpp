

#ifndef THE_QUEST_GAMESTATE_HPP
#define THE_QUEST_GAMESTATE_HPP


#pragma once
#include "Player.hpp"
#include "InputManager.hpp"
#include "State.hpp"
#include "StateManager.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include "Tilemap.hpp"
#include "tmxlite/Map.hpp"
#include "GameOverState.hpp"


class GameState : public State {
public:
    GameState(StateManager& manager);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }




private:
    StateManager& manager;
    sf::RenderWindow window;
    InputManager input;
    Player player;
    sf::Sprite sprite;
    TileMap map;
    sf::View view; //gameplay camera
    sf::View full = map.getFullMapView(); //full map
    sf::Texture *coinTexture;
    std::vector<Projectile> projectiles;
    sf::Texture* projectileTexture = nullptr;
    std::vector<Enemy> enemies;

    //HUD
    sf::Sprite heartSprite;
    sf::Texture* heartTexture;
    sf::Sprite coinSprite;
    sf::Texture* hdCoinTexture;
    sf::Font font;
    sf::Text coinText;
    sf::Clock clock; //measure time between heart damages
    int coinCount = 0;
};



#endif //THE_QUEST_GAMESTATE_HPP