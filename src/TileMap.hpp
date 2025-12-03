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
#include <unordered_map>

struct ObjectData {
    std::string name;
    std::string type;
    sf::FloatRect bounds;
};

struct TileObject {
    sf::Sprite sprite;
    sf::FloatRect bounds; // for collision
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
    //void loadPlatforms();

    //object groups for gameplay/collisions
    std::vector<sf::FloatRect> platforms;
    std::vector<sf::FloatRect> hazards;
    std::vector<sf::FloatRect> collectables;
    std::vector<sf::FloatRect> enemies;
    std::vector<sf::FloatRect> levelLogic;

    const std::vector<sf::FloatRect>& getPlatforms() const { return platforms; }
    const std::vector<sf::FloatRect>& getHazards() const { return hazards; }
    const std::vector<sf::FloatRect>& getCollectables() const { return collectables; }
    const std::vector<sf::FloatRect>& getEnemies() const { return enemies; }
    const std::vector<sf::FloatRect>& getLevelLogic() const { return levelLogic; }

    //renderable object sprites for drawing
    std::vector<TileObject> platformSprites;
    std::vector<TileObject> hazardSprites;
    std::vector<TileObject> enemySprites;
    std::vector<TileObject> collectableSprites;
    std::vector<TileObject> levelLogicSprites;


private:
    tmx::Map m_map; //tmxlite map object
    std::vector<sf::Sprite> m_drawOrder; // sprites in correct order
    std::string m_workingDir; // tmx file directory

    // cache textures loaded by ResourceManager (key = resolved path)
    std::unordered_map<std::string, sf::Texture*> m_textureCache;

    void loadTileAndImageLayers();
    void loadObjectLayers();

    const tmx::Tileset* findTilesetForGID(uint32_t gid) const;
    std::string resolvePath(const std::string& rel) const;

    //draw override
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
