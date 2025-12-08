//
// Created by Tanatswa Mlandeli on 08/12/2025.
//
#include "BackgroundMusic.hpp"
#include <iostream>

BackgroundMusic* BackgroundMusic::instance = nullptr;

BackgroundMusic& BackgroundMusic::getInstance() {
    if (!instance) {
        instance = new BackgroundMusic();
    }
    return *instance;
}

BackgroundMusic::~BackgroundMusic() {
    if (music.getStatus() == sf::Music::Playing) {
        music.stop();
    }
    delete instance;
    instance = nullptr;
}

void BackgroundMusic::load(const std::string& filename) {
    std::vector<std::string> paths = {
        filename,
        "../" + filename,
        "../../" + filename,
        "/Users/tanatswamlandeli/Documents/GroupW-Set09121/" + (filename.find("resources/") == 0 ? filename : "resources/sounds/" + filename)
    };

    for (const auto& path : paths) {
        if (music.openFromFile(path)) {
            std::cout << "Background music loaded from: " << path << std::endl;
            music.setLoop(true);
            return;
        }
    }
    std::cerr << "ERROR: Could not load background music: " << filename << std::endl;
}

void BackgroundMusic::play(float volume) {
    if (music.getStatus() != sf::Music::Playing) {
        music.setVolume(volume);
        music.play();
        std::cout << "Background music started playing" << std::endl;
    }
}

void BackgroundMusic::pause() {
    if (music.getStatus() == sf::Music::Playing) {
        music.pause();
    }
}

void BackgroundMusic::stop() {
    if (music.getStatus() == sf::Music::Playing) {
        music.stop();
    }
}

void BackgroundMusic::setVolume(float volume) {
    music.setVolume(volume);
}

bool BackgroundMusic::isPlaying() const {
    return music.getStatus() == sf::Music::Playing;
}