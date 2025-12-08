#include "Kingdom2LevelState.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

#include "GameOverState.hpp"
#include "KingdomSelectionState.hpp"
#include "LevelSelectState.hpp"
#include "MainMenuState.hpp"

Kingdom2LevelState::Kingdom2LevelState(StateManager& manager, KingdomID kingdom, LevelID level, int levelNumber)
    : manager(manager), levelNumber(levelNumber), kingdom(kingdom), level(level)
{
    font.loadFromFile("resources/fonts/MedievalSharp-Regular.ttf");

    enemies.clear();
    projectiles.clear();
    map.platforms.clear();
    map.hazards.clear();
    map.collectables.clear();
    map.levelLogic.clear();

    loadLevel();

    //hud
    projectileTexture = resources.getTexture("resources/tilesets/Sprites/Tiles/surge.png");
    coinTexture = resources.getTexture("resources/tilesets/Sprites/Tiles/Default/coin_gold.png");
    heartTexture = resources.getTexture("resources/tilesets/Sprites/Tiles/Default/hud_heart.png");
    hdCoinTexture = resources.getTexture("resources/tilesets/Sprites/Tiles/Default/hud_coin.png");

    if (!font.loadFromFile("resources/fonts/Vipnagorgialla Rg.otf"))
        std::cerr << "Failed to load HUD font\n";

    coinText.setFont(font);
    coinText.setCharacterSize(32);
    coinText.setFillColor(sf::Color::Black);

    heartSprite.setTexture(*heartTexture);
    coinSprite.setTexture(*hdCoinTexture);

    full = map.getFullMapView();

    isPaused = false;
    std::string options[3] = {"Resume", "Restart", "Menu"};
    for (int i = 0; i < 3; ++i) {
        pauseOptions[i].setFont(font);
        pauseOptions[i].setString(options[i]);
        pauseOptions[i].setCharacterSize(40);
        pauseOptions[i].setFillColor(sf::Color::White);
        pauseOptions[i].setOutlineColor(sf::Color::Black);
        pauseOptions[i].setOutlineThickness(2);
        pauseOptions[i].setPosition(500, 300 + i * 60);
    }

    // Pause text
    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(60);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setOutlineColor(sf::Color::Black);
    pauseText.setOutlineThickness(3);
    pauseText.setPosition(450.f, 150.f);

    // Pause overlay
    pauseOverlay.setSize(sf::Vector2f(1280.f, 720.f));
    pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));
}

void Kingdom2LevelState::loadLevel() {
    // Load TMX map based on levelNumber
    std::string tmxPath = "resources/tilemaps/lvl" + std::to_string(levelNumber) + "_2.tmx";
    if (!map.load(tmxPath)) std::cerr << "Failed to load tilemap: " << tmxPath << "\n";


    if (kingdom == KingdomID::Kingdom2) {
        switch (level) {
            case LevelID::Level1: map.load("resources/tilemaps/lvl1_2.tmx");
                break;
            case LevelID::Level2: map.load("resources/tilemaps/lvl2_2.tmx");
                break;
            case LevelID::Level3: map.load("resources/tilemaps/lvl3_2.tmx");
                break;
        }
    }

    enemies.clear();
    projectiles.clear();
    player.reset();

    // Place player at spawn
    sf::Vector2f spawn = map.getPlayerSpawn();
    player.setPosition(spawn);

    loadEnemies();
}

void Kingdom2LevelState::loadEnemies() {
    enemies.clear();
    for (const auto& spawn : map.getEnemies()) {
        EnemyType type = getEnemyTypeFromGID(spawn.gid);
        Enemy enemy(type, KingdomTheme::Kingdom2, spawn.bounds.left, spawn.bounds.top);
        enemies.push_back(std::move(enemy));
    }
}

EnemyType Kingdom2LevelState::getEnemyTypeFromGID(uint32_t gid) {
    switch(gid) {
        case 78: return EnemyType::Patrol;
        case 514: return EnemyType::Patrol;
        case 104: return EnemyType::Static;
        case 81: return EnemyType::Jumping;
        default: return EnemyType::Static;
    }
}

void Kingdom2LevelState::handleInput(sf::RenderWindow& window){
    sf::Event event;
    while (window.pollEvent(event))
    {
        player.handleInput();

        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
            isPaused = !isPaused;
            continue; // Skip game logic when paused
        }

        // when gameplay is paused
        if (isPaused) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                        pauseMenuIndex = (pauseMenuIndex + 2) % 3;
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        pauseMenuIndex = (pauseMenuIndex + 1) % 3;
                    }

                    if (event.key.code == sf::Keyboard::Enter) {

                        // resume
                        if (pauseMenuIndex == 0) {
                            isPaused = false;
                        }

                        // restart
                        else if (pauseMenuIndex == 1) {
                            if (kingdom == KingdomID::Kingdom1) {
                                manager.push(std::make_unique<Kingdom1LevelState>(manager, kingdom, level, levelNumber));
                            } else if (kingdom == KingdomID::Kingdom2) {
                                manager.push(std::make_unique<Kingdom2LevelState>(manager, kingdom, level, levelNumber));
                            }
                            return;
                        }

                        // menu
                        else if (pauseMenuIndex == 2) {
                            manager.push(std::make_unique<MainMenuState>(manager));
                            return;
                        }
                    }
            }
            // Do not continue game logic when paused
            continue;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::E) {

                float dir = player.isFacingRight() ? 1.f : -1.f;
                sf::Vector2f pos = player.getPosition();

                float x = pos.x + (dir * 30.f);
                float y = pos.y + player.getBounds().height * 0.65f;

                projectiles.emplace_back(x, y, dir, projectileTexture);
            }
        }
    }
}

void Kingdom2LevelState::update(float dt) {
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
            player.damage(); //lose a life
            break;
        }
    }

    //coin collection
    for (auto it = map.collectables.begin(); it != map.collectables.end();) {
        if (playerBounds.intersects(*it)) {
            std::cout << "Collected coin!\n";
            player.addCoin(); //increase coin count
            it = map.collectables.erase(it); //remove coin
        }
        else ++it;
    }

    //enemy collision
    for (auto& e : enemies) {
        if (!e.alive) continue;

        if (playerBounds.intersects(e.getBounds())) {
            std::cout << "Player hit by enemy!\n";
            player.damage();
            // player bounces back
            sf::Vector2f knockback = player.isFacingRight() ? sf::Vector2f(-30.f, -20.f) : sf::Vector2f(30.f, -20.f);
            player.getSprite().move(knockback);
            break;
        }
    }

    // projectile and enemy collision
    for (auto p = projectiles.begin(); p != projectiles.end();) {
        p->update(dt);

        bool projectileRemoved = false;  // mark if we erased projectile

        for (auto e = enemies.begin(); e != enemies.end();) {
            if (p->getBounds().intersects(e->getBounds())) {
                std::cout << "Enemy has been defeated!\n";

                // Remove enemy
                e = enemies.erase(e);

                // Remove projectile
                p = projectiles.erase(p);
                projectileRemoved = true;

                break;  // break out of enemy loop — projectile is gone
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
            if (!pendingStateChange) {
                pendingStateChange = true;
                stateDelayClock.restart();
            }
            if (pendingStateChange && stateDelayClock.getElapsedTime().asSeconds() >= 0.5f) {
                if (level == LevelID::Level1)
                    manager.push(std::make_unique<Kingdom2LevelState>(manager, KingdomID::Kingdom2, LevelID::Level2, 2));
                else if (level == LevelID::Level2)
                    manager.push(std::make_unique<Kingdom2LevelState>(manager, KingdomID::Kingdom2, LevelID::Level3, 3));
                else
                    manager.push(std::make_unique<LevelSelectState>(manager, 2));
                pendingStateChange = false;
            }
            break;
        }
    }


    //switch to gameoverstate if player dies
    if (player.dead()) {
        //manager.pop();
        manager.push(std::make_unique<GameOverState>(manager, kingdom, level, levelNumber));
        return;
    }

    //platform collision
    for (const sf::FloatRect& p : map.getPlatforms())
    {
        // check intersection
        if (playerBounds.intersects(p))
        {
            // ---- LANDING ON PLATFORM (coming from above) ----
            if (playerVel.y > 0 && playerBounds.top + playerBounds.height - 5 < p.top)
            {
                // snap player on top of platform
                playerPos.y = p.top - playerBounds.height;
                player.setPosition(playerPos);

                player.stopVertical();
                groundedThisFrame = true;
            }

            // (optional) prevent hitting head on platform
            else if (playerVel.y < 0 && playerBounds.top > p.top + p.height - 5)
            {
                playerPos.y = p.top + p.height;
                player.setPosition(playerPos);

                player.stopVertical();
            }
        }
    }
    player.setOnGround(groundedThisFrame);

    // Update enemies (movement & animation)
    for (auto& e : enemies)
        e.update(dt);

    // Update projectiles
    for (auto& p : projectiles)
        p.update(dt);


    if (input.isKeyPressedOnce(sf::Keyboard::Escape)) {
        // open pause menu
    }
}

void Kingdom2LevelState::render(sf::RenderWindow& window) {
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

    // Draw hearts (fade individually as health decreases)
    for (int i = 0; i < 3; ++i) {
        heartSprite.setPosition(10.f + i * 50.f, 10.f);

        if (i < player.getLives()) {
            heartSprite.setColor(sf::Color(255, 255, 255, 255)); // visible
        } else {
            heartSprite.setColor(sf::Color(255, 255, 255, 70)); // faded
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
    if (isPaused) {
        sf::RectangleShape darken(sf::Vector2f(window.getSize()));
        darken.setFillColor(sf::Color(0,0,0,150));
        window.draw(darken);

        for (int i = 0; i < 3; ++i) {
            pauseOptions[i].setFillColor(i == pauseMenuIndex ? sf::Color::Yellow : sf::Color::White);
            window.draw(pauseOptions[i]);

        }
    }
}