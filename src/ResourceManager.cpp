#include "ResourceManager.hpp"
#include <iostream>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

sf::Texture* ResourceManager::getTexture(const std::string& filename) {
    // Already loaded?
    auto it = m_textures.find(filename);
    if (it != m_textures.end())
        return &it->second;

    // Load fresh
    sf::Texture tex;
    if (!tex.loadFromFile(filename)) {
        std::cerr << "[ResourceManager] Failed to load texture: " << filename << "\n";
        return nullptr;
    }

    tex.setSmooth(false);

    // Insert into map
    m_textures[filename] = tex;

    return &m_textures[filename];
}

sf::Font* ResourceManager::getFont(const std::string& filename) {
    // Already loaded?
    auto it = m_fonts.find(filename);
    if (it != m_fonts.end())
        return &it->second;

    // Load new font
    sf::Font font;
    if (!font.loadFromFile(filename)) {
        std::cerr << "[ResourceManager] Failed to load font: " << filename << "\n";
        return nullptr;
    }

    m_fonts[filename] = font;
    return &m_fonts[filename];
}


// --- New Sound Buffer Loading ---
sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& filename) {
    // Already loaded?
    auto it = m_soundBuffers.find(filename);
    if (it != m_soundBuffers.end())
        return &it->second;

    // Load new sound buffer
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "[ResourceManager] Failed to load sound: " << filename << "\n";
        return nullptr;
    }

    // Insert into map
    m_soundBuffers[filename] = buffer;
    return &m_soundBuffers[filename];
}

// --- Play Sound Effect ---
void ResourceManager::playSound(const std::string& filename, float volume, bool loop) {
    // Get or load the sound buffer
    sf::SoundBuffer* buffer = getSoundBuffer(filename);
    if (!buffer) {
        std::cerr << "[ResourceManager] Cannot play sound, buffer not loaded: " << filename << "\n";
        return;
    }

    // Stop existing sound with this filename if playing
    auto soundIt = m_sounds.find(filename);
    if (soundIt != m_sounds.end() && soundIt->second) {
        soundIt->second->stop();
    }

    // Create new sound
    auto sound = std::make_unique<sf::Sound>();
    sound->setBuffer(*buffer);
    sound->setVolume(volume * (m_soundVolume / 100.0f));
    sound->setLoop(loop);
    sound->play();

    // Store in map
    m_sounds[filename] = std::move(sound);
}

// --- Helper for one-shot sounds ---
void ResourceManager::playSoundImmediate(const std::string& filename, float volume) {
    // Create a temporary sound for one-shot effects
    sf::SoundBuffer* buffer = getSoundBuffer(filename);
    if (!buffer) {
        return;
    }

    sf::Sound sound;
    sound.setBuffer(*buffer);
    sound.setVolume(volume * (m_soundVolume / 100.0f));
    sound.play();

    // Keep it alive while playing (simplified approach)
    while (sound.getStatus() == sf::Sound::Playing) {
        // This is a simple implementation - in practice, you might want
        // to manage this differently with your game loop
        sf::sleep(sf::milliseconds(10));
    }
}

// --- Stop Specific Sound ---
void ResourceManager::stopSound(const std::string& filename) {
    auto it = m_sounds.find(filename);
    if (it != m_sounds.end() && it->second) {
        it->second->stop();
    }
}

// --- Music Control ---
void ResourceManager::playMusic(const std::string& filename, bool loop, float volume) {
    // If already playing this music, just adjust volume/loop
    if (m_music && m_currentMusicFile == filename) {
        m_music->setLoop(loop);
        m_music->setVolume(volume * (m_musicVolume / 100.0f));
        return;
    }

    // Stop current music if playing
    if (m_music) {
        m_music->stop();
    }

    // Load new music
    m_music = std::make_unique<sf::Music>();
    if (!m_music->openFromFile(filename)) {
        std::cerr << "[ResourceManager] Failed to load music: " << filename << "\n";
        m_music.reset();
        return;
    }

    m_currentMusicFile = filename;
    m_music->setLoop(loop);
    m_music->setVolume(volume * (m_musicVolume / 100.0f));
    m_music->play();
}

void ResourceManager::stopMusic() {
    if (m_music) {
        m_music->stop();
    }
}

void ResourceManager::pauseMusic() {
    if (m_music) {
        m_music->pause();
    }
}

void ResourceManager::resumeMusic() {
    if (m_music) {
        m_music->play();
    }
}

void ResourceManager::setMusicVolume(float volume) {
    m_musicVolume = volume;
    if (m_music) {
        m_music->setVolume(volume);
    }
}

void ResourceManager::setSoundVolume(float volume) {
    m_soundVolume = volume;
    // Update volume for all active sounds
    for (auto& pair : m_sounds) {
        if (pair.second) {
            pair.second->setVolume(pair.second->getVolume() * (volume / 100.0f));
        }
    }
}

// cleanup
void ResourceManager::clear() {
    // Stop all sounds
    for (auto& pair : m_sounds) {
        if (pair.second) {
            pair.second->stop();
        }
    }

    // Stop music
    if (m_music) {
        m_music->stop();
    }

    // Clear containers
    m_textures.clear();
    m_sounds.clear();
    m_soundBuffers.clear();
    m_music.reset();
}
