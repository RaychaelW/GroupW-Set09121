#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "State.hpp"
#include "Player.hpp"
#include "Obstacle.hpp"

class GameState : public State {
public:
    GameState(StateManager& manager);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    StateManager& manager;
    Player player;

    std::vector<std::unique_ptr<Obstacle>> obstacles;

    float spawnTimer = 0.f;
    float spawnInterval = 2.f;

    bool playerCollidingLastFrame = false;

    void spawnObstacle();
};