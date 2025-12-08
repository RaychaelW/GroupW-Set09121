//
// Created by Tanatswa Mlandeli on 08/12/2025.
//

#pragma once
#include <string>

class GlobalSettings {
public:
    static GlobalSettings& getInstance();

    // Getters
    int getMasterVolume() const { return masterVolume; }
    bool isMusicOn() const { return musicOn; }
    bool isSFXOn() const { return sfxOn; }
    bool isFullscreen() const { return fullscreen; }
    bool needsFullscreenToggle() const { return toggleFullscreen; }

    // Setters
    void setMasterVolume(int vol);
    void setMusicOn(bool on);
    void setSFXOn(bool on);
    void setFullscreen(bool full);

    // Reset toggle flag
    void resetFullscreenToggle() { toggleFullscreen = false; }

    // Save/load
    void save();
    void load();

private:
    GlobalSettings();

    int masterVolume;
    bool musicOn;
    bool sfxOn;
    bool fullscreen;
    bool toggleFullscreen; // Flag to indicate we need to toggle fullscreen
};
