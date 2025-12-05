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


void ResourceManager::clear() {
    m_textures.clear();
}
