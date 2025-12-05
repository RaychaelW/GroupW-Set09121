#include "GameState.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include "Enemy.hpp"
#include "Projectile.hpp"
#include "GameOverState.hpp"


GameState::GameState(StateManager& manager)
    : manager(manager) {

    if (!map.load("resources/tilemaps/lvl1.tmx")) {
        std::cerr << " ERROR: Failed to load tilemap in Gamestate\n";
    }

    projectileTexture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Tiles/surge.png");
    if (!projectileTexture) {
        std::cerr << "Failed to load projectile texture\n";
    }

    for (auto& r : map.getEnemies()) {
        enemies.emplace_back(EnemyType::Static,
                             ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Enemies/Default/slime_normal_walk_a.png"),
                             r.left,
                             r.top);
    }


    coinTexture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Tiles/Default/coin_gold.png");
    coinTexture->setSmooth(false);

    //full map view
    full = map.getFullMapView();
}


void GameState::handleInput(sf::RenderWindow& window) {

    sf::Event event{};

    while (window.pollEvent(event)) {

        input.update(); //use Input manager
        player.handleInput(); // let the player read input

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Tab) {
                static bool usingFull = false;
                usingFull = !usingFull;
                window.setView(usingFull ? full : view);
            }
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::E) {
                float dir = player.isFacingRight() ? 1.f : -1.f;
                sf::Vector2f pos = player.getSprite().getPosition();

                // spawn slightly in front of player (tweak offsets)
                float spawnX = pos.x + (dir * 30.f);
                float spawnY = pos.y + player.getSprite().getGlobalBounds().height * 0.65f;

                projectiles.emplace_back(spawnX, spawnY, dir, projectileTexture);
            }
        }

        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            manager.pop();   // Return to previous state (menu)
        }
    }
}


void GameState::update(float dt) {
    input.update();
    player.update(dt);

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
            // we already erased projectile — don't increment p
            continue;
        }
        ++p;
    }


    //end of level door logic
    for (const sf::FloatRect& door : map.getLevelLogic()) {
        if (playerBounds.intersects(door)) {
            std::cout << "Level Complete!\n";
            //push rewards page (not implemented yet)
            break;
        }
    }


    //switch to gameoverstate if player dies
    if (player.dead()) {
        //manager.pop();
        manager.push(std::make_unique<GameOverState>(manager, window));
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


    if (input.isKeyPressedOnce(sf::Keyboard::Escape)) {
        // open pause menu
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

    //enemies.draw(window);

    //window.setView(window.getDefaultView());
    //ui.draw(window);

}
