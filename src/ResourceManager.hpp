//
// Created by prais on 28/11/2025.
//

#ifndef THE_QUEST_RESOURCEMANAGER_HPP
#define THE_QUEST_RESOURCEMANAGER_HPP

#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <SFML/Audio.hpp>
#include <memory>

class ResourceManager
{
public:
    static ResourceManager& getInstance();

    sf::Texture* getTexture(const std::string& filename);
    sf::Font* getFont(const std::string& filename);

    // Sound methods
    sf::SoundBuffer* getSoundBuffer(const std::string& filename);
    void playSound(const std::string& filename, float volume = 100.0f, bool loop = false);
    void stopSound(const std::string& filename);

    // Music methods
    void playMusic(const std::string& filename, bool loop = true, float volume = 50.0f);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    void setMusicVolume(float volume);
    void setSoundVolume(float volume);

    void clear();


private:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    void playSoundImmediate(const std::string& filename, float volume = 100.0f);

    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<std::string, sf::Font> m_fonts;

    // New sound storage
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> m_sounds;

    // Music
    std::unique_ptr<sf::Music> m_music;
    std::string m_currentMusicFile;
    float m_musicVolume = 50.0f;
    float m_soundVolume = 100.0f;
};



#endif //THE_QUEST_RESOURCEMANAGER_HPP