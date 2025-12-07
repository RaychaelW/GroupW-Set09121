#ifndef THE_QUEST_ENEMY_HPP
#define THE_QUEST_ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include <iostream>
#include <tmxlite/Map.hpp>

enum class EnemyType {
    Patrol,
    Static,
    Jumping
};

enum class KingdomTheme {
    Kingdom1,
    Kingdom2,
    Kingdom3
};

class Enemy {
public:
    Enemy() = default;
    Enemy(EnemyType type, KingdomTheme theme, float x, float y);

    void update(float dt);
    void render(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    bool alive = true;
    void takeDamage(int d = 1) { hp -= d; if (hp <= 0) alive = false; }

    void loadTextureForType(EnemyType type, KingdomTheme theme);
    void initAnimation(int frameWidth, int frameHeight, int numFrames);


private:
    EnemyType type;
    KingdomTheme theme = KingdomTheme::Kingdom1;


    sf::Sprite sprite;
    const sf::Texture* texture = nullptr;
    int hp = 1;
    float direction = 1.f;

    float speed = 90.f;
    float leftLimit = 0.f;
    float rightLimit = 0.f;

    //jumping mechanics
    float vVel = 0.f;
    float gravity = 900.f;
    float jumpTimer = 0.f;
    float jumpInterval = 2.0f;

    //animation
    std::vector<sf::IntRect> frames;
    float animTimer = 0.f;
    float animInterval = 0.2f; //time per frame
    int currentFrame = 0;

};

#endif //THE_QUEST_ENEMY_HPP
