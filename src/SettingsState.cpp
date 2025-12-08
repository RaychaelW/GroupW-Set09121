#include "SettingsState.hpp"
#include <iostream>
#include <fstream>
#include <SFML/Audio.hpp>
#include "GlobalSettings.hpp"

#include "MainMenuState.hpp"

// Custom clamp function (works without C++17)
int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

SettingsState::SettingsState(StateManager& manager)
    : manager(manager)
{
    font.loadFromFile("resources/fonts/MedievalSharp-Regular.ttf");

    // Create a sophisticated dark gradient background
    backgroundImage.create(1280, 720);

    // Dark blue to deep purple gradient with subtle texture
    for (int y = 0; y < 720; ++y) {
        for (int x = 0; x < 1280; ++x) {
            float gradient = y / 720.0f;

            // Base dark colors
            int r = 15 + static_cast<int>(5 * gradient);
            int g = 20 + static_cast<int>(10 * gradient);
            int b = 40 + static_cast<int>(20 * gradient);

            // Add subtle noise for texture
            int noise = (x * 73 + y * 97) % 7 - 3;
            r = clamp(r + noise, 10, 40);
            g = clamp(g + noise, 15, 45);
            b = clamp(b + noise, 35, 70);

            backgroundImage.setPixel(x, y, sf::Color(r, g, b));
        }
    }
    backgroundTexture.loadFromImage(backgroundImage);
    backgroundSprite.setTexture(backgroundTexture);

    // Create decorative grid lines
    gridImage.create(1280, 720, sf::Color::Transparent);
    for (int y = 0; y < 720; ++y) {
        for (int x = 0; x < 1280; ++x) {
            if (x % 40 == 0 || y % 40 == 0) {
                sf::Color current = backgroundImage.getPixel(x, y);
                // Brighten the grid lines slightly
                gridImage.setPixel(x, y, sf::Color(
                    current.r + 10 < 255 ? current.r + 10 : 255,
                    current.g + 10 < 255 ? current.g + 10 : 255,
                    current.b + 15 < 255 ? current.b + 15 : 255,
                    50
                ));
            }
        }
    }
    gridTexture.loadFromImage(gridImage);
    gridSprite.setTexture(gridTexture);

    // Title - Gold with dark outline
    title.setFont(font);
    title.setString("SETTINGS");
    title.setCharacterSize(56);
    title.setStyle(sf::Text::Bold);
    title.setPosition(130, 40);
    title.setFillColor(sf::Color(255, 215, 0)); // Gold
    title.setOutlineThickness(3);
    title.setOutlineColor(sf::Color(20, 20, 40));

    loadSettings();

    options.resize(5);
    optionValues.resize(5);

    // Labels with consistent styling
    options[0] = sf::Text("Master Volume", font, 36);
    options[1] = sf::Text("Music", font, 36);
    options[2] = sf::Text("SFX", font, 36);
    options[3] = sf::Text("Fullscreen", font, 36);
    options[4] = sf::Text("Reset Progress", font, 36);

    for (int i = 0; i < options.size(); i++) {
        options[i].setPosition(140, 160 + i * 70);
        options[i].setOutlineThickness(1);
        options[i].setOutlineColor(sf::Color(30, 30, 60));
    }

    optionValues[0] = std::to_string(masterVolume);
    optionValues[1] = musicOn ? "On" : "Off";
    optionValues[2] = sfxOn ? "On" : "Off";
    optionValues[3] = fullscreen ? "On" : "Off";
    optionValues[4] = "Press Enter";

    updateHighlight();
}

void SettingsState::handleInput(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {

            // ESC = Go Back
            if (event.key.code == sf::Keyboard::Escape) {
                manager.push(std::make_unique<MainMenuState>(manager));
                return;
            }

            // Menu navigation
            if (event.key.code == sf::Keyboard::Up) {
                selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
                updateHighlight();
            }

            if (event.key.code == sf::Keyboard::Down) {
                selectedIndex = (selectedIndex + 1) % options.size();
                updateHighlight();
            }

            bool right = (event.key.code == sf::Keyboard::Right);
            bool left  = (event.key.code == sf::Keyboard::Left);

            if (right || left) {
                auto& settings = GlobalSettings::getInstance();

                if (selectedIndex == 0) {
                    int newVolume = masterVolume + (right ? 5 : -5);
                    if (newVolume < 0) newVolume = 0;
                    if (newVolume > 100) newVolume = 100;
                    masterVolume = newVolume;
                    settings.setMasterVolume(masterVolume);
                }
                else if (selectedIndex == 1) {
                    musicOn = !musicOn;
                    settings.setMusicOn(musicOn);
                }
                else if (selectedIndex == 2) {
                    sfxOn = !sfxOn;
                    settings.setSFXOn(sfxOn);
                }
                else if (selectedIndex == 3) {
                    fullscreen = !fullscreen;
                    settings.setFullscreen(fullscreen);
                }

                optionValues[0] = std::to_string(masterVolume);
                optionValues[1] = musicOn ? "On" : "Off";
                optionValues[2] = sfxOn ? "On" : "Off";
                optionValues[3] = fullscreen ? "On" : "Off";
            }

            // ENTER KEY
            if (event.key.code == sf::Keyboard::Enter) {
                // Reset progress
                if (selectedIndex == 4) {
                    std::ofstream ofs("save/progress.dat");
                    ofs << "";  // truncate file
                    ofs.close();
                    optionValues[4] = "Progress Reset!";
                }
            }
        }
    }
}

void SettingsState::update(float dt) {}

void SettingsState::render(sf::RenderWindow& window) {
    // Draw background and grid
    window.draw(backgroundSprite);
    window.draw(gridSprite);

    // Draw decorative header bar
    sf::RectangleShape header(sf::Vector2f(1280, 120));
    header.setFillColor(sf::Color(20, 25, 45, 180));
    window.draw(header);

    // Draw decorative footer
    sf::RectangleShape footer(sf::Vector2f(1280, 60));
    footer.setPosition(0, 540);
    footer.setFillColor(sf::Color(15, 20, 35, 200));
    window.draw(footer);

    window.draw(title);

    // Draw options with better visual separation
    for (int i = 0; i < options.size(); i++) {
        // Draw subtle background for each option
        sf::RectangleShape optionBg(sf::Vector2f(720, 45));
        optionBg.setPosition(130, 155 + i * 70);
        optionBg.setFillColor(sf::Color(30, 35, 55, 120));
        optionBg.setOutlineThickness(1);
        optionBg.setOutlineColor(sf::Color(50, 55, 85));
        window.draw(optionBg);

        window.draw(options[i]);

        sf::Text value(optionValues[i], font, 32);
        value.setPosition(520, options[i].getPosition().y + 5);
        value.setFillColor(selectedIndex == i ? sf::Color(255, 215, 0) : sf::Color(200, 210, 230));
        value.setOutlineThickness(1);
        value.setOutlineColor(sf::Color(30, 30, 60));

        window.draw(value);
    }

    // Control hints with better styling
    sf::Text hint("ESC = Back | Left/Right = Adjust", font, 20);
    hint.setFillColor(sf::Color(180, 185, 210));
    hint.setOutlineColor(sf::Color(30, 30, 60));
    hint.setOutlineThickness(1);
    hint.setPosition(100, 560);
    window.draw(hint);
}

void SettingsState::updateHighlight()
{
    for (int i = 0; i < options.size(); i++) {
        if (i == selectedIndex) {
            options[i].setFillColor(sf::Color(255, 215, 0)); // Gold
            options[i].setOutlineColor(sf::Color(180, 150, 0));
        } else {
            options[i].setFillColor(sf::Color(220, 230, 240)); // Light blue-gray
            options[i].setOutlineColor(sf::Color(30, 30, 60));
        }
    }
}

void SettingsState::applySettings()
{
    std::ofstream out("config/settings.cfg");

    out << "masterVolume=" << masterVolume << "\n";
    out << "musicOn=" << (musicOn ? 1 : 0) << "\n";
    out << "sfxOn=" << (sfxOn ? 1 : 0) << "\n";
    out << "fullscreen=" << (fullscreen ? 1 : 0) << "\n";
}

void SettingsState::loadSettings()
{
    std::ifstream in("config/settings.cfg");
    if (!in) return;

    std::string line;

    while (std::getline(in, line)) {

        if (line.rfind("masterVolume=", 0) == 0)
            masterVolume = std::stoi(line.substr(13));

        if (line.rfind("musicOn=", 0) == 0)
            musicOn = (std::stoi(line.substr(8)) != 0);

        if (line.rfind("sfxOn=", 0) == 0)
            sfxOn = (std::stoi(line.substr(6)) != 0);

        if (line.rfind("fullscreen=", 0) == 0)
            fullscreen = (std::stoi(line.substr(11)) != 0);
    }
}