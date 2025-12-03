#include "GameState.hpp"
#include "ResourceManager.hpp"
#include <iostream>


GameState::GameState(StateManager& manager)
    : manager(manager)
{
    if (!map.load("resources/tilemaps/lvl1.tmx")) {
        std::cerr << " ERROR: Failed to load tilemap in Gamestate\n";
    }

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


    if (input.isKeyPressedOnce(sf::Keyboard::Escape)) {
        // open pause menu
    }
}


void GameState::render(sf::RenderWindow& window) {
    window.setView(full);
    window.draw(map);
    player.render(window);

    //enemies.draw(window);

    //window.setView(window.getDefaultView());
    //ui.draw(window);

}
