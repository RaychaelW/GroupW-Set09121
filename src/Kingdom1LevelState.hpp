
#ifndef THE_QUEST_KINGDOM1LEVELSTATE_HPP
#define THE_QUEST_KINGDOM1LEVELSTATE_HPP


#include <unordered_map>

#include "State.hpp"
#include "StateManager.hpp"
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include "Enemy.hpp"
#include "InputManager.hpp"
#include "Player.hpp"
#include "ResourceManager.hpp"
#include "Tilemap.hpp"
#include <vector>

enum class KingdomID {
    Kingdom1,
    Kingdom2
};

enum class LevelID {
    Level1,
    Level2,
    Level3
};

class Kingdom1LevelState : public State {
public:
    Kingdom1LevelState(StateManager& manager, KingdomID kingdom, LevelID level, int levelNumber = 1);

    void handleInput(sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;



private:
    //helps with different enemy behaviours
    struct LevelData {
        std::string mapFile;
        std::unordered_map<int, EnemyType> enemyGIDToType;
    };

    std::vector<LevelData> levels;
    int currentLevel = 0;
    int levelNumber = 1;

    KingdomID kingdom;
    LevelID level;

    // Internal helpers
    void loadLevel();
    void spawnEnemiesForLevel(const LevelData& data);
    void loadEnemies();
    EnemyType getEnemyTypeFromGID(uint32_t gid);

    StateManager& manager;
    sf::RenderWindow window;
    InputManager input;
    Player player;
    sf::Sprite sprite;
    TileMap map;
    sf::View view; //gameplay camera
    sf::View full = map.getFullMapView(); //full map
    sf::Texture *coinTexture;
    std::vector<Projectile> projectiles;
    sf::Texture* projectileTexture = nullptr;
    std::vector<Enemy> enemies;
    sf::Texture* enemyTexture = nullptr;
    sf::Clock stateDelayClock;
    bool pendingStateChange = false;

    //HUD
    sf::Sprite heartSprite;
    sf::Texture* heartTexture;
    sf::Sprite coinSprite;
    sf::Texture* hdCoinTexture;
    sf::Font font;
    sf::Text coinText;
    sf::Clock clock; //measure time between heart damages
    int coinCount = 0;

    ResourceManager& resources = ResourceManager::getInstance();
};



#endif //THE_QUEST_KINGDOM1LEVELSTATE_HPP