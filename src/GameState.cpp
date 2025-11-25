#include "GameState.hpp"
#include <SFML/Window/Event.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

GameState::GameState(StateManager& manager) : manager(manager) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void GameState::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    player.handleInput();
}

void GameState::update(float dt) {
    player.update(dt);

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnObstacle();
        spawnTimer = 0.f;
    }

    // Update obstacles
    for (auto& obs : obstacles)
        obs->update(dt, scrollSpeed);

    // Remove off-screen obstacles
    obstacles.erase(
        std::remove_if(obstacles.begin(), obstacles.end(),
                       [](const std::unique_ptr<Obstacle>& o) { return o->isOffScreen(); }),
        obstacles.end());

    // Collision detection
    for (auto& obs : obstacles) {
        if (player.getBounds().intersects(obs->getBounds())) {
            std::cout << "Player hit an obstacle!" << std::endl;
        }
    }
}

void GameState::render(sf::RenderWindow& window) {
    player.render(window);
    for (auto& obs : obstacles)
        obs->render(window);
}

void GameState::spawnObstacle() {
    float y;
    std::string file;

    int type = rand() % 4; // 0=tree,1=rock,2=stick,3=bird

    switch(type) {
        case 0: file = "resources/textures/tree.png"; y = 450.f; break;
        case 1: file = "resources/textures/rock.png"; y = 470.f; break;
        case 2: file = "resources/textures/bird.png"; y = 300.f + rand() % 100; break;
    }

    float x = 800.f; // off-screen right
    obstacles.push_back(std::make_unique<Obstacle>(x, y, file, 50.f, 50.f));
}
