#pragma once
#include "State.hpp"
#include "Player.hpp"
#include "Obstacle.hpp"
#include "StateManager.hpp"
#include <vector>
#include <memory>

class GameState : public State {
public:
    GameState(StateManager& manager);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void spawnObstacle();

    StateManager& manager;
    Player player;
    std::vector<std::unique_ptr<Obstacle>> obstacles;

    float spawnTimer = 0.f;
    float spawnInterval = 1.5f;
    float scrollSpeed = 300.f;
};
