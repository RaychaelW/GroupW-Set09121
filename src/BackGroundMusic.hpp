//
// Created by Tanatswa Mlandeli on 08/12/2025.
//

#pragma once
#include <SFML/Audio.hpp>
#include <string>

class BackgroundMusic {
private:
    sf::Music music;
    static BackgroundMusic* instance;

public:
    static BackgroundMusic& getInstance();

    void load(const std::string& filename);
    void play(float volume = 40.0f);
    void pause();
    void stop();
    void setVolume(float volume);
    bool isPlaying() const;

private:
    BackgroundMusic() = default;
    ~BackgroundMusic();
    BackgroundMusic(const BackgroundMusic&) = delete;
    BackgroundMusic& operator=(const BackgroundMusic&) = delete;
};
