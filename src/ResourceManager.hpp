//
// Created by prais on 28/11/2025.
//

#ifndef THE_QUEST_RESOURCEMANAGER_HPP
#define THE_QUEST_RESOURCEMANAGER_HPP

#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class ResourceManager
{
public:
    static ResourceManager& getInstance();

    sf::Texture* getTexture(const std::string& filename);
    void clear(); // optional


private:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::unordered_map<std::string, sf::Texture> m_textures;
};



#endif //THE_QUEST_RESOURCEMANAGER_HPP