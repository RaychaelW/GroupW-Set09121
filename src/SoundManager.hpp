#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

class SoundManager {
private:
    std::map<std::string, sf::SoundBuffer> buffers;
    std::vector<sf::Sound> activeSounds;
    float masterVolume;

public:
    static SoundManager& getInstance();

    void loadSound(const std::string& name, const std::string& filename);
    void playSound(const std::string& name, float volume = 100.f, float pitch = 1.f);
    void stopAllSounds();
    
    void setMasterVolume(float volume);
    float getMasterVolume() const;
    
    void cleanup();
    
private:
    SoundManager();
    ~SoundManager();
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
};