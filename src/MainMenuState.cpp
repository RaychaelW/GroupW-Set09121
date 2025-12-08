#include "MainMenuState.hpp"
#include "GameState.hpp"
#include "SoundManager.hpp"
#include <iostream>
#include "HowToPlayState.hpp"
#include "KingdomSelectionState.hpp"
#include "SettingsState.hpp"
#include <SFML/Audio.hpp>
#include "BackGroundMusic.hpp"
#include <fstream>

// Function to load settings from file
void loadAndApplySettings() {
    std::ifstream in("config/settings.cfg");
    if (!in) {
        std::cout << "Settings file not found, using defaults" << std::endl;
        return;
    }

    std::string line;
    int masterVolume = 100;
    int musicOn = 1;
    int sfxOn = 1;

    while (std::getline(in, line)) {
        if (line.rfind("masterVolume=", 0) == 0)
            masterVolume = std::stoi(line.substr(13));
        if (line.rfind("musicOn=", 0) == 0)
            musicOn = std::stoi(line.substr(8));
        if (line.rfind("sfxOn=", 0) == 0)
            sfxOn = std::stoi(line.substr(6));
    }

    // Apply to SoundManager
    auto& soundManager = SoundManager::getInstance();
    soundManager.setMasterVolume(static_cast<float>(masterVolume));


    std::cout << "Applied settings: Master=" << masterVolume
              << ", SFX=" << (sfxOn ? "On" : "Off") << std::endl;
}
MainMenuState::MainMenuState(StateManager& manager)
    : manager(manager){

    font.loadFromFile("resources/fonts/MedievalSharp-Regular.ttf");

    backgroundTexture.loadFromFile("resources/tilesets/Sprites/Backgrounds/bg.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(1.f, 1.f);

    // Title
    title.setFont(font);
    title.setString("THE QUEST");
    title.setCharacterSize(68);
    title.setStyle(sf::Text::Bold);
    title.setPosition(450, 80);
    title.setOutlineThickness(3);
    title.setOutlineColor(sf::Color::Black);

    // Play
    playText.setFont(font);
    playText.setString("Play");
    playText.setCharacterSize(40);
    playText.setPosition(500, 250);
    playText.setOutlineThickness(2);
    playText.setOutlineColor(sf::Color::Black);

    // Settings
    settingsText.setFont(font);
    settingsText.setString("Settings");
    settingsText.setCharacterSize(40);
    settingsText.setPosition(500, 330);
    settingsText.setOutlineThickness(2);
    settingsText.setOutlineColor(sf::Color::Black);

    // How To Play
    howToPlayText.setFont(font);
    howToPlayText.setString("How to Play");
    howToPlayText.setCharacterSize(40);
    howToPlayText.setPosition(500, 410);
    howToPlayText.setOutlineThickness(2);
    howToPlayText.setOutlineColor(sf::Color::Black);

    // Quit
    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(40);
    quitText.setPosition(500, 490);
    quitText.setOutlineThickness(2);
    quitText.setOutlineColor(sf::Color::Black);

    options = { &playText, &settingsText, &howToPlayText, &quitText };

    updateSelection();

    loadAndApplySettings();

    // BACKGROUND MUSIC - Apply music setting
    static sf::Music backgroundMusic;
    static bool musicLoaded = false;
    if (!musicLoaded) {
        std::vector<std::string> musicPaths = {
            "resources/sounds/GameBg.wav",
            "../resources/sounds/GameBg.wav",
            "../../resources/sounds/GameBg.wav",
        };
    }
    // START BACKGROUND MUSIC
    auto& bgMusic = BackgroundMusic::getInstance();
    bgMusic.load("resources/sounds/GameBg.wav");
    bgMusic.play(40.0f);  // 40% volume
}

void MainMenuState::handleInput(sf::RenderWindow& window) {
    sf::Event event{};
    while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {

            // ESC does nothing here
            if (event.key.code == sf::Keyboard::Escape) {}

            if (event.key.code == sf::Keyboard::Up) {
                selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
                updateSelection();

                // CHECK SFX SETTING BEFORE PLAYING
                bool sfxEnabled = true;
                std::ifstream settings("config/settings.cfg");
                if (settings) {
                    std::string line;
                    while (std::getline(settings, line)) {
                        if (line.rfind("sfxOn=", 0) == 0) {
                            sfxEnabled = (std::stoi(line.substr(6)) != 0);
                            break;
                        }
                    }
                }


                // PLAY SOUND
                static sf::SoundBuffer jumpBuffer;
                static bool jumpLoaded = false;
                static sf::Sound jumpSound;

                if (!jumpLoaded) {
                    // Try multiple paths
                    std::vector<std::string> jumpPaths = {
                        "resources/sounds/Jump.wav",
                        "/Users/tanatswamlandeli/Documents/GroupW-Set09121/resources/sounds/Jump.wav"
                    };

                    for (const auto& path : jumpPaths) {
                        if (jumpBuffer.loadFromFile(path)) {
                            jumpLoaded = true;
                            jumpSound.setBuffer(jumpBuffer);
                            std::cout << "Loaded Jump.wav from: " << path << std::endl;
                            break;
                        }
                    }
                }

                if (jumpLoaded) {
                    jumpSound.setVolume(30.0f);
                    jumpSound.play();
                }
            }

            if (event.key.code == sf::Keyboard::Down) {
                selectedIndex = (selectedIndex + 1) % options.size();
                updateSelection();

                // PLAY SOUND - Reuse same jump sound
                static sf::SoundBuffer jumpBuffer;
                static bool jumpLoaded = false;
                static sf::Sound jumpSound;

                if (!jumpLoaded) {
                    std::vector<std::string> jumpPaths = {
                        "../../resources/sounds/Jump.wav",
                        "../resources/sounds/Jump.wav",
                        "resources/sounds/Jump.wav",
                        "/Users/tanatswamlandeli/Documents/GroupW-Set09121/resources/sounds/Jump.wav"
                    };

                    for (const auto& path : jumpPaths) {
                        if (jumpBuffer.loadFromFile(path)) {
                            jumpLoaded = true;
                            jumpSound.setBuffer(jumpBuffer);
                            std::cout << "Loaded Jump.wav from: " << path << std::endl;
                            break;
                        }
                    }
                }

                if (jumpLoaded) {
                    jumpSound.setVolume(30.0f);
                    jumpSound.play();
                }
            }

            if (event.key.code == sf::Keyboard::Enter) {

                // PLAY CONFIRM SOUND
                static sf::SoundBuffer coinBuffer;
                static bool coinLoaded = false;
                static sf::Sound coinSound;

                if (!coinLoaded) {
                    std::vector<std::string> coinPaths = {
                        "resources/sounds/Coin.wav",
                        "resources/sounds/Coin.wav"
                    };

                    for (const auto& path : coinPaths) {
                        if (coinBuffer.loadFromFile(path)) {
                            coinLoaded = true;
                            coinSound.setBuffer(coinBuffer);
                            std::cout << "Loaded Coin.wav from: " << path << std::endl;
                            break;
                        }
                    }
                }

                if (coinLoaded) {
                    coinSound.setVolume(60.0f);
                    coinSound.play();
                }

                if (selectedIndex == 0) {
                    manager.push(std::make_unique<KingdomSelectionState>(manager));
                }
                else if (selectedIndex == 1) {
                    manager.push(std::make_unique<SettingsState>(manager));
                }
                else if (selectedIndex == 2) {
                    manager.push(std::make_unique<HowToPlayState>(manager));
                }
                else if (selectedIndex == 3) {
                    // PLAY DIFFERENT SOUND FOR QUIT
                    static sf::SoundBuffer jumpBuffer;
                    static bool jumpLoaded = false;
                    static sf::Sound quitSound;

                    if (!jumpLoaded) {
                        std::vector<std::string> jumpPaths = {
                            "../../resources/sounds/Jump.wav",
                            "../resources/sounds/Jump.wav",
                            "resources/sounds/Jump.wav",
                            "/Users/tanatswamlandeli/Documents/GroupW-Set09121/resources/sounds/Jump.wav"
                        };

                        for (const auto& path : jumpPaths) {
                            if (jumpBuffer.loadFromFile(path)) {
                                jumpLoaded = true;
                                quitSound.setBuffer(jumpBuffer);
                                std::cout << "Loaded Jump.wav for quit from: " << path << std::endl;
                                break;
                            }
                        }
                    }

                    if (jumpLoaded) {
                        quitSound.setVolume(50.0f);
                        quitSound.play();

                        BackgroundMusic::getInstance().stop();

                        // Small delay before closing to hear sound
                        sf::Clock delayClock;
                        while (delayClock.getElapsedTime().asSeconds() < 0.3f) {
                            // Wait briefly
                        }
                    }

                    window.close();
                }
            }
        }
    }
}

void MainMenuState::update(float dt) {}

void MainMenuState::render(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(title);
    window.draw(playText);
    window.draw(settingsText);
    window.draw(howToPlayText);
    window.draw(quitText);
}

void MainMenuState::updateSelection() {
    for (auto* t : options)
        t->setFillColor(sf::Color::White);

    options[selectedIndex]->setFillColor(sf::Color::Yellow);
}