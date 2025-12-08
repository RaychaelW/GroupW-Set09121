#include "SoundManager.hpp"

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

SoundManager::SoundManager() : masterVolume(100.f) {}

SoundManager::~SoundManager() {
    cleanup();
}

void SoundManager::loadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(filename)) {
        buffers[name] = buffer;
    } else {
        std::cerr << "ERROR: Failed to load sound: " << filename << std::endl;
    }
}

void SoundManager::playSound(const std::string& name, float volume, float pitch) {
    if (buffers.find(name) != buffers.end()) {
        // Create a new sound
        sf::Sound sound;
        sound.setBuffer(buffers[name]);
        sound.setVolume(volume * (masterVolume / 100.f));
        sound.setPitch(pitch);
        sound.play();

        // Store in active sounds
        activeSounds.push_back(sound);

        // Clean up finished sounds
        activeSounds.erase(
            std::remove_if(activeSounds.begin(), activeSounds.end(),
                [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }),
            activeSounds.end()
        );
    }
}

void SoundManager::stopAllSounds() {
    for (auto& sound : activeSounds) {
        sound.stop();
    }
    activeSounds.clear();
}

void SoundManager::setMasterVolume(float volume) {
    masterVolume = std::max(0.f, std::min(100.f, volume));
    // Volume changes apply to new sounds only with this implementation
}

float SoundManager::getMasterVolume() const {
    return masterVolume;
}

void SoundManager::cleanup() {
    stopAllSounds();
    buffers.clear();
}