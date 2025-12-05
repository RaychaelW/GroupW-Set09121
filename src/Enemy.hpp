#ifndef THE_QUEST_ENEMY_HPP
#define THE_QUEST_ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include <iostream>
#include <tmxlite/Map.hpp>
#include "Tilemap.hpp"

enum class EnemyType {
    Patrol,
    Static,
    Jumping,
    Flying,
    Shooter
};

class Enemy {
public:
    Enemy() = default;
    Enemy(EnemyType t, const sf::Texture* texture, float x, float y);

    void update(float dt);
    void render(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    bool alive = true;
    void takeDamage(int d = 1) { hp -= d; if (hp <= 0) alive = false; }

    // public configuration
    float speed = 90.f;
    float leftLimit = 0.f;
    float rightLimit = 0.f;


private:
    EnemyType type = EnemyType::Static;
    sf::Sprite sprite;
    int hp = 1;

    //jumping mechanics
    float vVel = 0.f;
    float gravity = 900.f;
    float jumpTimer = 0.f;
    float jumpInterval = 2.0f;

};

#endif //THE_QUEST_ENEMY_HPP
