#include "GameState.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include "GameOverState.hpp"


GameState::GameState(StateManager& manager)
    : manager(manager) {
    if (!map.load("resources/tilemaps/lvl1.tmx")) {
        std::cerr << " ERROR: Failed to load tilemap in Gamestate\n";
    }

    coinTexture.loadFromFile("resources/tilesets/Sprites/Tiles/Default/coin_gold.png");
    coinTexture.setSmooth(false);

    //full map view
    full = map.getFullMapView();
}


void GameState::handleInput(sf::RenderWindow& window) {

    sf::Event event{};

    while (window.pollEvent(event)) {

        input.update(); //use Input manager
        player.handleInput(); // let the player read input(WASD)

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Tab) {
                static bool usingFull = false;
                usingFull = !usingFull;
                window.setView(usingFull ? full : view);
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
    for (const sf::FloatRect& e : map.getEnemies()) {
        if (playerBounds.intersects(e)) {
            player.damage(); //lose a life and hit animation
            break;
        }
    }


    //end of level door logic
    for (const sf::FloatRect& door : map.getLevelLogic()) {
        if (playerBounds.intersects(door)) {
            std::cout << "Level Complete!\n";
            //push rewards page (not implemented yet)
            break;
        }
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
        coin.setTexture(coinTexture);
        coin.setPosition(c.left, c.top);
        window.draw(coin);
    }

    //enemies.draw(window);

    //window.setView(window.getDefaultView());
    //ui.draw(window);

}
