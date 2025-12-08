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
    std::string basePath;

public:
    static SoundManager& getInstance();

    // Make sure all these methods are declared
    void loadSound(const std::string& name, const std::string& filename);

    // Two overloaded playSound methods
    void playSound(const std::string& filename, float volume = 100.f, float pitch = 1.f);
    void playSound(const std::string& name, float volume = 100.f);

    void cleanup();  // Make sure this is declared

    void setMasterVolume(float volume);
    float getMasterVolume() const;
    
private:
    SoundManager();
    ~SoundManager();
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
};