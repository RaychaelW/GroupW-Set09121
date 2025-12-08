//
// Created by Tanatswa Mlandeli on 08/12/2025.
//
#include "GlobalSettings.hpp"
#include <fstream>
#include <iostream>

GlobalSettings& GlobalSettings::getInstance() {
    static GlobalSettings instance;
    return instance;
}

GlobalSettings::GlobalSettings()
    : masterVolume(100), musicOn(true), sfxOn(true), fullscreen(false), toggleFullscreen(false) {
    load();
}

void GlobalSettings::setMasterVolume(int vol) {
    masterVolume = vol;
    if (masterVolume < 0) masterVolume = 0;
    if (masterVolume > 100) masterVolume = 100;
    save();
}

void GlobalSettings::setMusicOn(bool on) {
    musicOn = on;
    save();
}

void GlobalSettings::setSFXOn(bool on) {
    sfxOn = on;
    save();
}

void GlobalSettings::setFullscreen(bool full) {
    if (fullscreen != full) {
        fullscreen = full;
        toggleFullscreen = true; // Set flag to toggle in main loop
    }
    save();
}

void GlobalSettings::save() {
    std::ofstream out("config/settings.cfg");
    out << "masterVolume=" << masterVolume << "\n";
    out << "musicOn=" << (musicOn ? 1 : 0) << "\n";
    out << "sfxOn=" << (sfxOn ? 1 : 0) << "\n";
    out << "fullscreen=" << (fullscreen ? 1 : 0) << "\n";
}

void GlobalSettings::load() {
    std::ifstream in("config/settings.cfg");
    if (!in) return;

    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind("masterVolume=", 0) == 0)
            masterVolume = std::stoi(line.substr(13));
        else if (line.rfind("musicOn=", 0) == 0)
            musicOn = (std::stoi(line.substr(8)) != 0);
        else if (line.rfind("sfxOn=", 0) == 0)
            sfxOn = (std::stoi(line.substr(6)) != 0);
        else if (line.rfind("fullscreen=", 0) == 0)
            fullscreen = (std::stoi(line.substr(11)) != 0);
    }
}