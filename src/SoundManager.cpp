#include "SoundManager.hpp"
#include <filesystem>

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

SoundManager::SoundManager() : masterVolume(100.f), basePath("/Users/tanatswamlandeli/Documents/GroupW-Set09121/") {}

SoundManager::~SoundManager() {
    cleanup();
}

void SoundManager::loadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;

    // Try different paths
    std::vector<std::string> paths = {
        filename,
        "../" + filename,
        "../../" + filename,
        basePath + filename,
        basePath + "../" + filename
    };

    for (const auto& path : paths) {
        if (buffer.loadFromFile(path)) {
            buffers[name] = buffer;
            std::cout << "Loaded sound '" << name << "' from: " << path << std::endl;
            return;
        }
    }

    std::cerr << "ERROR: Could not load sound '" << name << "' from: " << filename << std::endl;
}

void SoundManager::playSound(const std::string& name, float volume) {
    if (buffers.find(name) != buffers.end()) {
        sf::Sound sound;
        sound.setBuffer(buffers[name]);
        sound.setVolume(volume * (masterVolume / 100.f));
        sound.play();

        activeSounds.push_back(sound);

        // Clean up finished sounds
        activeSounds.erase(
            std::remove_if(activeSounds.begin(), activeSounds.end(),
                [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }),
            activeSounds.end()
        );
    } else {
        std::cout << "WARNING: Sound '" << name << "' not loaded" << std::endl;
    }
}

// ADD THIS METHOD - It was declared but not defined
void SoundManager::cleanup() {
    // Stop all active sounds
    for (auto& sound : activeSounds) {
        sound.stop();
    }
    activeSounds.clear();

    // Clear all buffers
    buffers.clear();

    std::cout << "SoundManager cleaned up" << std::endl;
}

void SoundManager::setMasterVolume(float volume) {
    masterVolume = std::max(0.f, std::min(100.f, volume));
    std::cout << "Master volume set to: " << masterVolume << std::endl;
}

float SoundManager::getMasterVolume() const {
    return masterVolume;
}

// If you have playSound with filename (not name), add this too:
void SoundManager::playSound(const std::string& filename, float volume, float pitch) {
    // This is the overloaded version that takes filename directly
    sf::SoundBuffer buffer;

    std::vector<std::string> paths = {
        filename,
        "../" + filename,
        "../../" + filename,
        basePath + filename
    };

    for (const auto& path : paths) {
        if (buffer.loadFromFile(path)) {
            sf::Sound sound;
            sound.setBuffer(buffer);
            sound.setVolume(volume * (masterVolume / 100.f));
            sound.setPitch(pitch);
            sound.play();

            activeSounds.push_back(sound);

            // Clean up finished sounds
            activeSounds.erase(
                std::remove_if(activeSounds.begin(), activeSounds.end(),
                    [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }),
                activeSounds.end()
            );

            std::cout << "Played sound from file: " << path << std::endl;
            return;
        }
    }

    std::cerr << "ERROR: Could not play sound from: " << filename << std::endl;
}