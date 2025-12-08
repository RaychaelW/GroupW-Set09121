#include "GameState.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include "Enemy.hpp"
#include "Projectile.hpp"
#include "GameOverState.hpp"
#include "Kingdom1LevelState.hpp"
#include "KingdomSelectionState.hpp"
#include <SFML/Audio.hpp>
#include "SoundManager.hpp"
#include "GlobalSettings.hpp"

// Helper function to play sounds with settings applied
void playSoundWithSettings(sf::Sound& sound, float baseVolume) {
    auto& settings = GlobalSettings::getInstance();

    // Check if SFX is enabled
    if (!settings.isSFXOn()) {
        return; // Don't play sound if SFX is off
    }

    // Calculate actual volume based on master volume
    float actualVolume = baseVolume * (settings.getMasterVolume() / 100.0f);

    // Apply volume and play
    sound.setVolume(actualVolume);
    sound.play();

    // Debug output
    // std::cout << "Playing sound: base=" << baseVolume << ", actual=" << actualVolume
    //           << ", masterVol=" << settings.getMasterVolume()
    //           << ", SFX=" << (settings.isSFXOn() ? "ON" : "OFF") << std::endl;
}

// Global background music (shared across all states)
static sf::Music* globalBackgroundMusic = nullptr;

GameState::GameState(StateManager& manager)
    : manager(manager) {

    // Load settings first
    auto& settings = GlobalSettings::getInstance();
    settings.load();

    if (!map.load("resources/tilemaps/lvl1.tmx")) {
        std::cerr << " ERROR: Failed to load tilemap in Gamestate\n";
    }

    //energy surge
    projectileTexture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Tiles/surge.png");
    if (!projectileTexture) {
        std::cerr << "Failed to load projectile texture\n";
    }

    //enemy
    for (auto& spawn : map.getEnemies()) {
        EnemyType type = map.getEnemyTypeFromGID(spawn.gid);

        enemies.emplace_back(
        type,
        KingdomTheme::Kingdom1, spawn.bounds.left, spawn.bounds.top);
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

    // BACKGROUND MUSIC - Shared across all states
    if (!globalBackgroundMusic) {
        globalBackgroundMusic = new sf::Music();

        std::vector<std::string> musicPaths = {
            "../../resources/sounds/GameBg.wav",
            "../resources/sounds/GameBg.wav",
            "resources/sounds/GameBg.wav"
        };

        bool musicLoaded = false;
        for (const auto& path : musicPaths) {
            if (globalBackgroundMusic->openFromFile(path)) {
                musicLoaded = true;
                std::cout << "Background music loaded from: " << path << std::endl;
                break;
            }
        }

        if (!musicLoaded) {
            // Try absolute path as last resort
            std::string absolutePath = "/Users/tanatswamlandeli/Documents/GroupW-Set09121/resources/sounds/GameBg.wav";
            if (globalBackgroundMusic->openFromFile(absolutePath)) {
                std::cout << "Background music loaded from absolute path" << std::endl;
            } else {
                std::cerr << "ERROR: Could not load background music" << std::endl;
                delete globalBackgroundMusic;
                globalBackgroundMusic = nullptr;
            }
        }
    }

    // Apply music settings
    if (globalBackgroundMusic) {
        globalBackgroundMusic->setLoop(true);

        // Apply volume based on settings
        float volume = settings.isMusicOn() ? (settings.getMasterVolume() * 0.4f) : 0.0f;
        globalBackgroundMusic->setVolume(volume);

        if (globalBackgroundMusic->getStatus() != sf::Music::Playing) {
            globalBackgroundMusic->play();
        }

        std::cout << "GameState: Music " << (settings.isMusicOn() ? "ON" : "OFF")
                  << ", Volume: " << volume << std::endl;
    }
}


void GameState::handleInput(sf::RenderWindow& window) {
    sf::Event event{};

    while (window.pollEvent(event)) {
        // Check for fullscreen toggle request
        auto& settings = GlobalSettings::getInstance();
        if (settings.needsFullscreenToggle()) {
            // Store current view
            sf::View currentView = window.getView();

            // Close and recreate window
            window.close();

            if (settings.isFullscreen()) {
                window.create(sf::VideoMode::getDesktopMode(), "THE QUEST", sf::Style::Fullscreen);
                std::cout << "Switched to Fullscreen" << std::endl;
            } else {
                window.create(sf::VideoMode(1280, 720), "THE QUEST", sf::Style::Default);
                std::cout << "Switched to Windowed" << std::endl;
            }

            // Restore settings
            window.setView(currentView);
            window.setFramerateLimit(60);

            // Reset the flag
            settings.resetFullscreenToggle();

            return; // Skip rest of input handling this frame
        }

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

                // Add projectile sound - WITH SETTINGS
                static sf::SoundBuffer projBuffer;
                static bool projLoaded = false;
                static sf::Sound projSound;

                if (!projLoaded) {
                    std::vector<std::string> projPaths = {
                        "../../resources/sounds/Projectile.wav",
                        "../resources/sounds/Projectile.wav",
                        "resources/sounds/Projectile.wav"
                    };

                    for (const auto& path : projPaths) {
                        if (projBuffer.loadFromFile(path)) {
                            projLoaded = true;
                            projSound.setBuffer(projBuffer);
                            break;
                        }
                    }
                }

                if (projLoaded) {
                    playSoundWithSettings(projSound, 70.0f);
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
            static float lastPress = 0;
            float now = clock.getElapsedTime().asSeconds();
            if (now - lastPress > 0.5f) { // prevents spamming
                player.damage();
                lastPress = now;
            }
        }

        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            manager.push(std::make_unique<KingdomSelectionState>(manager));   // Return to kingdom selection
        }
    }
}


void GameState::update(float dt) {
    // Update background music volume based on settings
    if (globalBackgroundMusic) {
        auto& settings = GlobalSettings::getInstance();
        float volume = settings.isMusicOn() ? (settings.getMasterVolume() * 0.4f) : 0.0f;
        globalBackgroundMusic->setVolume(volume);
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
            // Coin sound is played in Player::addCoin()

            it = map.collectables.erase(it); //remove coin
        }
        else ++it;
    }

    //enemy collision
    for (auto& e : enemies) {
        if (!e.alive) continue;

        if (playerBounds.intersects(e.getBounds())) {
            std::cout << "Player hit by enemy!\n";
            player.damage(); // Hurt sound is played in Player::damage()

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

                // Add enemy hit sound - WITH SETTINGS
                static sf::SoundBuffer enemyHitBuffer;
                static bool enemyHitLoaded = false;
                static sf::Sound enemyHitSound;

                if (!enemyHitLoaded) {
                    std::vector<std::string> hitPaths = {
                        "../../resources/sounds/EnemyHit.wav",
                        "../resources/sounds/EnemyHit.wav",
                        "resources/sounds/EnemyHit.wav"
                    };

                    for (const auto& path : hitPaths) {
                        if (enemyHitBuffer.loadFromFile(path)) {
                            enemyHitLoaded = true;
                            enemyHitSound.setBuffer(enemyHitBuffer);
                            break;
                        }
                    }
                }

                if (enemyHitLoaded) {
                    playSoundWithSettings(enemyHitSound, 80.0f);
                }

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

            // Add level complete sound - WITH SETTINGS
            static sf::SoundBuffer levelCompleteBuffer;
            static bool levelCompleteLoaded = false;
            static sf::Sound levelCompleteSound;

            if (!levelCompleteLoaded) {
                std::vector<std::string> completePaths = {
                    "../../resources/sounds/LevelCompleted.wav",
                    "../resources/sounds/LevelCompleted.wav",
                    "resources/sounds/LevelCompleted.wav"
                };

                for (const auto& path : completePaths) {
                    if (levelCompleteBuffer.loadFromFile(path)) {
                        levelCompleteLoaded = true;
                        levelCompleteSound.setBuffer(levelCompleteBuffer);
                        break;
                    }
                }
            }

            if (levelCompleteLoaded) {
                playSoundWithSettings(levelCompleteSound, 90.0f);
            }

            if (!pendingStateChange) {
                pendingStateChange = true;
                stateDelayClock.restart();
            }
            if (pendingStateChange && stateDelayClock.getElapsedTime().asSeconds() >= 0.5f) {
                manager.push(std::make_unique<Kingdom1LevelState>(manager, KingdomID::Kingdom1, LevelID::Level1));
                pendingStateChange = false;

            }
            break;
        }
    }


    //switch to gameoverstate if player dies
    if (player.dead()) {
        // Death sound is played in Player::damage() when lives reach 0

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

}

// Clean up global music when game ends
GameState::~GameState() {
    // We'll keep the music playing for other states
    // It will be cleaned up when the program ends
}