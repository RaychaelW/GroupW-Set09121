#include "Enemy.hpp"

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include <tmxlite/Map.hpp>
#include "Tilemap.hpp"

Enemy::Enemy(EnemyType t, const sf::Texture* texture, float x, float y)
: type(t)
{
    if (texture) sprite.setTexture(*texture);
    sprite.setPosition(x, y);
    sprite.setOrigin(0.f, 0.f);

    // sensible default patrol bounds around spawn
    leftLimit = x - 80.f;
    rightLimit = x + 80.f;
}

void Enemy::update(float dt) {
    if (!alive) return;

    switch (type) {
        case EnemyType::Patrol: {
            // move horizontally, reverse at bounds
            sprite.move(speed * dt, 0.f);
            float px = sprite.getPosition().x;
            if (px < leftLimit) {
                sprite.setPosition(leftLimit, sprite.getPosition().y);
                speed = std::abs(speed); // go right
            } else if (px > rightLimit) {
                sprite.setPosition(rightLimit, sprite.getPosition().y);
                speed = -std::abs(speed); // go left
            }
            break;
        }
        case EnemyType::Static:
            // no movement
            break;

        case EnemyType::Jumping: {
            jumpTimer += dt;
            if (jumpTimer >= jumpInterval) {
                jumpTimer = 0;
                vVel = -300.f;
            }
            vVel += gravity * dt;
            sprite.move(0.f, vVel * dt);
            break;
        }
        case EnemyType::Flying: {
            // simple bobbing motion
            static float t = 0.f;
            t += dt;
            float dy = std::sin(t * 2.f) * 6.f * dt * 60.f;
            sprite.move(0.f, dy);
            break;
        }
        case EnemyType::Shooter:
            // may have its own logic; left empty for now
            break;
        default:
            break;
    }
}

void Enemy::render(sf::RenderWindow& window) const {
    if (alive) window.draw(sprite);
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}
