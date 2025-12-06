#include "GameState.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include "Enemy.hpp"
#include "Projectile.hpp"
#include "GameOverState.hpp"
#include "KingdomSelectionState.hpp"

GameState::GameState(StateManager& manager)
    : manager(manager) {

    if (!map.load("resources/tilemaps/lvl1.tmx")) {
        std::cerr << " ERROR: Failed to load tilemap in Gamestate\n";
    }

    //energy surge
    projectileTexture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Tiles/surge.png");
    if (!projectileTexture) {
        std::cerr << "Failed to load projectile texture\n";
    }

    //enemy
    for (auto& r : map.getEnemies()) {
        enemies.emplace_back(EnemyType::Static,
                             ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Enemies/Default/slime_normal_walk_a.png"),
                             r.left, r.top);
    }

    //gameplay coin for collection
    coinTexture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Tiles/Default/coin_gold.png");
    coinTexture->setSmooth(false);

    //HUD heart and coin icons - load textures
    heartTexture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Tiles/Default/hud_heart.png");
    if (!heartTexture)
        std::cerr << "Failed to load heart texture\n";
    hdCoinTexture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Tiles/Default/hud_coin.png");
    if (!heartTexture)
        std::cerr << "Failed to load coin texture\n";

    heartSprite.setTexture(*heartTexture);
    heartSprite.setScale(1.0f, 1.0f);
    coinSprite.setTexture(*hdCoinTexture);
    coinSprite.setScale(1.0f, 1.0f);

    // Load font for HUD text
    if (!font.loadFromFile("resources/fonts/Vipnagorgialla Rg.otf"))
        std::cerr << "Failed to load font" << std::endl;

    coinText.setFont(font);
    coinText.setCharacterSize(32);
    coinText.setFillColor(sf::Color::Black);

    //full map view
    full = map.getFullMapView();

    // Initialize pause system
    isPaused = false;


    // Pause text
    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(60);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setOutlineColor(sf::Color::Black);
    pauseText.setOutlineThickness(3);
    pauseText.setPosition(400.f, 200.f);

    // Pause overlay
    pauseOverlay.setSize(sf::Vector2f(1280.f, 720.f));
    pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    ResourceManager::getInstance().playMusic("resources/sounds/GameBg.wav", true, 30.0f);
}

void GameState::handleInput(sf::RenderWindow& window) {
    sf::Event event{};

    while (window.pollEvent(event)) {
        input.update();

        // Pause/Unpause with P key
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::P) {
                togglePause();
                if (isPaused) continue;
            }
        }

        // If game is paused, only handle unpause
        if (isPaused) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    togglePause();
                }
            }
            continue;
        }

        player.handleInput();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Tab) {
                static bool usingFull = false;
                usingFull = !usingFull;
                window.setView(usingFull ? full : view);
                ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 30.0f);
            }
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::E) {
                float dir = player.isFacingRight() ? 1.f : -1.f;
                sf::Vector2f pos = player.getSprite().getPosition();

                float spawnX = pos.x + (dir * 30.f);
                float spawnY = pos.y + player.getSprite().getGlobalBounds().height * 0.65f;

                projectiles.emplace_back(spawnX, spawnY, dir, projectileTexture);

                ResourceManager::getInstance().playSound("resources/sounds/Jump.wav", 60.0f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
            static float lastPress = 0;
            float now = clock.getElapsedTime().asSeconds();
            if (now - lastPress > 0.5f) {
                player.damage();
                lastPress = now;
                ResourceManager::getInstance().playSound("resources/sounds/Jump.wav", 70.0f);
            }
        }

        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 50.0f);
            manager.push(std::make_unique<KingdomSelectionState>(manager));
        }
    }
}

void GameState::update(float dt) {
    if (isPaused) {
        return;
    }

    input.update();
    player.update(dt);

    coinText.setString(std::to_string(player.getCoins()));

    sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();
    sf::Vector2f playerPos = player.getSprite().getPosition();
    sf::Vector2f playerVel = player.getVelocity();
    bool groundedThisFrame = false;

    //hazard collision
    for (const sf::FloatRect& h : map.getHazards()) {
        if (playerBounds.intersects(h)) {
            player.damage();
            ResourceManager::getInstance().playSound("resources/sounds/Jump.wav", 80.0f);
            break;
        }
    }

    //coin collection
    for (auto it = map.collectables.begin(); it != map.collectables.end();) {
        if (playerBounds.intersects(*it)) {
            std::cout << "Collected coin!\n";
            player.addCoin();
            ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 70.0f);
            it = map.collectables.erase(it);
        }
        else ++it;
    }

    //enemy collision
    for (auto& e : enemies) {
        if (!e.alive) continue;

        if (playerBounds.intersects(e.getBounds())) {
            std::cout << "Player hit by enemy!\n";
            player.damage();
            ResourceManager::getInstance().playSound("resources/sounds/Jump.wav", 90.0f);

            sf::Vector2f knockback = player.isFacingRight() ? sf::Vector2f(-30.f, -20.f) : sf::Vector2f(30.f, -20.f);
            player.getSprite().move(knockback);
            break;
        }
    }

    // projectile and enemy collision
    for (auto p = projectiles.begin(); p != projectiles.end();) {
        p->update(dt);

        bool projectileRemoved = false;

        for (auto e = enemies.begin(); e != enemies.end();) {
            if (p->getBounds().intersects(e->getBounds())) {
                std::cout << "Enemy has been defeated!\n";

                ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 80.0f);

                e = enemies.erase(e);

                p = projectiles.erase(p);
                projectileRemoved = true;

                break;
            } else {
                ++e;
            }
        }

        if (projectileRemoved) {
            continue;
        }
        ++p;
    }

    //end of level door logic
    for (const sf::FloatRect& door : map.getLevelLogic()) {
        if (playerBounds.intersects(door)) {
            std::cout << "Level Complete!\n";
            ResourceManager::getInstance().playSound("resources/sounds/LevelCompleted.wav", 100.0f);
            break;
        }
    }

    //switch to gameoverstate if player dies
    if (player.dead()) {
        ResourceManager::getInstance().playSound("resources/sounds/Jump.wav", 100.0f);
        manager.push(std::make_unique<GameOverState>(manager, window));
        return;
    }

    //platform collision
    for (const sf::FloatRect& p : map.getPlatforms())
    {
        if (playerBounds.intersects(p))
        {
            if (playerVel.y > 0 && playerBounds.top + playerBounds.height - 5 < p.top)
            {
                playerPos.y = p.top - playerBounds.height;
                player.setPosition(playerPos);

                player.stopVertical();
                groundedThisFrame = true;
            }

            else if (playerVel.y < 0 && playerBounds.top > p.top + p.height - 5)
            {
                playerPos.y = p.top + p.height;
                player.setPosition(playerPos);

                player.stopVertical();
            }
        }
    }
    player.setOnGround(groundedThisFrame);

    if (input.isKeyPressedOnce(sf::Keyboard::Escape)) {
        // open pause menu
    }
}

void GameState::togglePause() {
    isPaused = !isPaused;

    if (isPaused) {
        ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 50.0f);
        ResourceManager::getInstance().pauseMusic();
    } else {
        ResourceManager::getInstance().playSound("resources/sounds/Coin.wav", 50.0f);
        ResourceManager::getInstance().resumeMusic();
    }
}

void GameState::render(sf::RenderWindow& window) {
    window.setView(full);
    window.draw(map);
    player.render(window);

    for (const auto& c : map.getCollectables()) {
        sf::Sprite coin;
        coin.setTexture(*coinTexture);
        coin.setPosition(c.left, c.top);
        window.draw(coin);
    }

    for (auto& p : projectiles)
        p.render(window);

    for (auto& e : enemies)
        e.render(window);

    window.setView(window.getDefaultView());

    // Draw hearts
    for (int i = 0; i < 3; ++i) {
        heartSprite.setPosition(10.f + i * 50.f, 10.f);

        if (i < player.getLives()) {
            heartSprite.setColor(sf::Color(255, 255, 255, 255));
        } else {
            heartSprite.setColor(sf::Color(255, 255, 255, 70));
        }
        window.draw(heartSprite);
    }

    // Draw coin HUD
    coinSprite.setPosition(1130.f, 10.f);
    window.draw(coinSprite);

    coinText.setPosition(1200.f, 20.f);
    window.draw(coinText);

    // Draw pause icon
    window.draw(pauseIconSprite);

    // Draw pause overlay and text if paused
    if (isPaused) {
        window.draw(pauseOverlay);
        window.draw(pauseText);
    }
}