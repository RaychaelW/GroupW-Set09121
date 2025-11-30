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

void ResourceManager::clear() {
    m_textures.clear();
}
