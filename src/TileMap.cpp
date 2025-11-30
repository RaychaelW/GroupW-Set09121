#include "TileMap.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>


// helper: get parent directory of a path string without using <filesystem>
static std::string getParentPath(const std::string &path) {
    if (path.empty()) return std::string();

    // Normalize separators: convert backslashes to slashes for consistency
    std::string p = path;
    std::replace(p.begin(), p.end(), '\\', '/');

    // If path ends with '/', remove trailing slash
    if (!p.empty() && p.back() == '/') p.pop_back();

    // find last slash
    auto pos = p.find_last_of('/');
    if (pos == std::string::npos) return std::string(); // no parent
    return p.substr(0, pos);
}

static std::string normalizePath(std::string path) {

    std::replace(path.begin(), path.end(), '\\', '/');

    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string item;

    while (std::getline(ss, item, '/')) {
        if (item == "" || item == ".") continue;
        if (item == "..") {
            if (!parts.empty()) parts.pop_back();
        } else {
            parts.push_back(item);
        }
    }

    std::string result;
    for (size_t i = 0; i < parts.size(); ++i) {
        result += parts[i];
        if (i + 1 < parts.size()) result += "/";
    }
    return result;
}


bool TileMap::load(const std::string& tmxFilePath) {
    m_workingDir = getParentPath(tmxFilePath);

    //load map
    if (!m_map.load(tmxFilePath)) {
        std::cout << "Failed to load TMX file: " << tmxFilePath << "\n";
        return false;
    }

    // clear old
    m_drawOrder.clear();
    platforms.clear();
    hazards.clear();
    collectables.clear();
    enemies.clear();
    levelLogic.clear();

    // load layers & objects
    loadTileAndImageLayers();
    loadObjectLayers();

    return true;
}


// Resolve an image-collection tileset for a given GID
const tmx::Tileset* TileMap::findTilesetForGID(uint32_t gid) const {
    if (gid == 0) return nullptr;
    const auto& tilesets = m_map.getTilesets();
    // iterate in reverse so higher firstGID wins when overlapping
    for (int i = static_cast<int>(tilesets.size()) - 1; i >= 0; --i) {
        const auto& ts = tilesets[i];
        if (gid >= ts.getFirstGID() && gid < ts.getFirstGID() + ts.getTileCount())
            return &ts;
    }
    return nullptr;
}
std::string TileMap::resolvePath(const std::string& rel) const {
    // if relative is absolute (starts with drive or slash) just normalize and return
    if (rel.empty()) return rel;
    // simple check for absolute windows path "C:/" or leading '/'
    if ((rel.size() >= 2 && std::isalpha(rel[0]) && rel[1] == ':') || rel.front() == '/') {
        return normalizePath(rel);
    }
    // otherwise join with working dir if present
    std::string joined;
    if (m_workingDir.empty()) joined = rel;
    else joined = m_workingDir + "/" + rel;
    return normalizePath(joined);
}


// Load all tile layers (image collection tilesets = each tile has its own PNG)
void TileMap::loadTileAndImageLayers() {

    auto mapSize = m_map.getTileCount();
    auto tileSize = m_map.getTileSize();
    const auto& layers = m_map.getLayers();

    for (const auto& layerPtr : layers) {

        // IMAGE LAYERS
        if (layerPtr->getType() == tmx::Layer::Type::Image) {

            const tmx::ImageLayer* imgLayer = &layerPtr->getLayerAs<tmx::ImageLayer>();
            if (!imgLayer) continue;

            std::string imgRel = imgLayer->getImagePath();
            if (imgRel.empty()) continue;

            std::string imgFull = resolvePath(imgRel);

            // load texture via ResourceManager
            sf::Texture *tex = ResourceManager::getInstance().getTexture(imgFull);
            if (!tex) break;

            sf::Sprite sprite;
            sprite.setTexture(*tex);

            sprite.setScale(1.f, 1.f); // no scaling!

            // preserve Tiled layer offset (can be fractional)
            auto off = imgLayer->getOffset();
            sprite.setPosition(off.x, off.y);

            m_drawOrder.push_back((sprite));
            continue;
        }

        // TILE LAYERS
        if (layerPtr->getType() == tmx::Layer::Type::Tile) {
            const tmx::TileLayer* tileLayer = &layerPtr->getLayerAs<tmx::TileLayer>();
            if (!tileLayer) continue;

            const auto tiles = tileLayer->getTiles();
            // iterate grid positions
            for (uint32_t y = 0; y < mapSize.y; ++y) {

                for (uint32_t x = 0; x < mapSize.x; ++x) {

                    size_t idx = static_cast<size_t>(y) * mapSize.x + x;
                    if (idx >= tiles.size()) continue;

                    const auto& tile = tiles[idx];
                    uint32_t gid = tile.ID; // global id (0 = empty)
                    if (gid == 0) continue;

                    const tmx::Tileset* ts = findTilesetForGID(gid);
                    if (!ts) continue;
                    uint32_t localID = gid - ts->getFirstGID();

                    sf::Sprite sprite;

                    // Case A: tileset is an image-collection (no main imagePath)
                    if (ts->getImagePath().empty()) {

                        // each tile has its own image
                        const auto tilesVec = ts->getTiles(); // vector<tmx::Tile>
                        if (localID >= tilesVec.size()) continue;
                        const auto& tinfo = tilesVec[localID];

                        std::string imgRel = tinfo.imagePath; // relative in TSX
                        if (imgRel.empty()) continue;

                        std::string imgFull = resolvePath(imgRel);
                        sf::Texture *tex = ResourceManager::getInstance().getTexture(imgFull);
                        if (!tex) continue;

                        sprite.setTexture(*tex);
                    }
                    else {
                        // Case B: tileset has a single image (atlas)
                        std::string atlasFull = resolvePath(ts->getImagePath());
                        sf::Texture *atlas = ResourceManager::getInstance().getTexture(atlasFull);
                        if (!atlas) continue;

                        // tile size from tileset
                        auto tsTileSize = ts->getTileSize();
                        unsigned columns = atlas->getSize().x / tsTileSize.x;
                        //if (tsTileSize.x > 0) columns = atlas->getSize().x / tsTileSize.x;

                        unsigned tu = localID % columns;
                        unsigned tv = localID / columns;

                        sprite.setTexture(*atlas);
                        sprite.setTextureRect(sf::IntRect(
                            tu * tsTileSize.x,
                            tv * tsTileSize.y,
                            tsTileSize.x,
                            tsTileSize.y
                        ));

                    }

                    sprite.setPosition(x * tileSize.x, y * tileSize.y);
                    m_drawOrder.push_back((sprite));
                }
            }
        }
    }
}


// Load object layers & classify them
void TileMap::loadObjectLayers() {

    for (const auto& layer : m_map.getLayers())
    {
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        // Extract real object-group
        const auto& objGroup = layer->getLayerAs<tmx::ObjectGroup>();
        std::string name = objGroup.getName();

        // Loop through objects
        for (const auto& obj : objGroup.getObjects()) {
            auto a = obj.getAABB();
            sf::FloatRect rect(a.left, a.top, a.width, a.height);

            // Push into correct list
            if (name == "platforms")
                platforms.push_back(rect);
            else if (name == "hazards")
                hazards.push_back(rect);
            else if (name == "collectables")
                collectables.push_back(rect);
            else if (name == "enemies")
                enemies.push_back(rect);
            else if (name == "level_logic")
                levelLogic.push_back(rect);
        }
    }
}


sf::Vector2u TileMap::getMapPixelSize() const {
    auto count = m_map.getTileCount();
    auto tsize = m_map.getTileSize();
    return sf::Vector2u(count.x * tsize.x, count.y * tsize.y);
}

sf::View TileMap::getFullMapView() const {
    auto px = getMapPixelSize();
    return sf::View(sf::FloatRect(0, 0, px.x, px.y));
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {

    states.transform *= getTransform();

    //draw in insertion order
    for (const auto& sprite : m_drawOrder)
        target.draw(sprite, states);

}
