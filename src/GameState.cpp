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

    player.handleInput(); // FIXED
}

void GameState::update(float dt) {
    player.update(dt);

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnObstacle();
        spawnTimer = 0.f;
    }

    for (auto& obs : obstacles)
        obs->update(dt);

    bool colliding = false;

    for (auto& obs : obstacles) {
        if (player.getBounds().intersects(obs->getBounds())) {
            colliding = true;
        }
    }

    if (colliding && !playerCollidingLastFrame) {
        std::cout << "Player hit an obstacle!" << std::endl;
    }

    playerCollidingLastFrame = colliding;
}

void GameState::render(sf::RenderWindow& window) {
    player.render(window);

    for (auto& obs : obstacles)
        obs->render(window);
}

void GameState::spawnObstacle() {
    float y;
    std::string file;

    int type = rand() % 3;

    switch (type) {
    case 0: file = "resources/textures/tree.png"; y = 450; break;
    case 1: file = "resources/textures/rock.png"; y = 470; break;
    case 2: file = "resources/textures/bird.png"; y = 300 + (rand()%100); break;
    }

    obstacles.push_back(
        std::make_unique<Obstacle>(800.f, y, file, 50.f, 50.f)
    );
}