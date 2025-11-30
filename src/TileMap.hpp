#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/ImageLayer.hpp>
#include "ResourceManager.hpp"
#include <vector>
#include <string>

struct ObjectData {
    std::string name;
    std::string type;
    sf::FloatRect bounds;
};

class TileMap : public sf::Drawable, public sf::Transformable {

public:
    TileMap() = default;
    ~TileMap() = default;

    //loads tmx file and returns true on success
    bool load(const std::string& tmxFilePath);

    //map size in px
    sf::Vector2u getMapPixelSize() const;
    // view that shows the whole map
    sf::View getFullMapView() const;

    //object groups
    std::vector<sf::FloatRect> platforms;
    std::vector<sf::FloatRect> hazards;
    std::vector<sf::FloatRect> collectables;
    std::vector<sf::FloatRect> enemies;
    std::vector<sf::FloatRect> levelLogic;

    //object layers for game logic
    const std::vector<sf::FloatRect>& getPlatforms() const { return platforms; }
    const std::vector<sf::FloatRect>& getHazards() const { return hazards; }
    const std::vector<sf::FloatRect>& getCollectables() const { return collectables; }
    const std::vector<sf::FloatRect>& getEnemies() const { return enemies; }
    const std::vector<sf::FloatRect>& getLevelLogic() const { return levelLogic; }


private:
    tmx::Map m_map; //tmxlite map object
    std::vector<sf::Sprite> m_drawOrder; // sprites in correct order
    std::string m_workingDir; // tmx file directory


    void loadTileAndImageLayers();
    void loadObjectLayers();

    const tmx::Tileset* findTilesetForGID(uint32_t gid) const;
    std::string resolvePath(const std::string& rel) const;

    //draw override
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
