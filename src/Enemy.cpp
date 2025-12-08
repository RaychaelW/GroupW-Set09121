#include "Enemy.hpp"

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include <tmxlite/Map.hpp>
#include "Tilemap.hpp"

Enemy::Enemy(EnemyType t, KingdomTheme k, float x, float y)
    : type(t), theme(k)
{
    loadTextureForType(t, k);
    sprite.setTexture(*texture);
    sprite.setPosition(x, y);
    sprite.setOrigin(0.f, 0.f);

    // sensible default patrol bounds around spawn
    leftLimit = x - 80.f;
    rightLimit = x + 80.f;

    startY = sprite.getPosition().y;
}

void Enemy::loadTextureForType(EnemyType type, KingdomTheme theme)
{
    switch (theme)
    {
        case KingdomTheme::Kingdom1:
            switch (type)
            {
            case EnemyType::Static:
                    texture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Spritesheets/spritesheet-enemies-default.png");
                    initAnimation(100, 100, 3);
                    frames= {{128, 213, 64, 44}, {127, 87, 64, 44}, {128, 150, 64, 44}};
                    break;

            case EnemyType::Patrol:
                    texture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Spritesheets/spritesheet-enemies-default.png");
                    initAnimation(100, 100, 3);
                    frames= {{321, 131, 64, 62}, {320, 194, 64, 62}, {320, 256, 64, 62}};
                    break;

            case EnemyType::Jumping:
                    texture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Spritesheets/spritesheet-enemies-default.png");
                    initAnimation(100, 100, 3);
                    frames= {{318, 70, 66, 60}, {319, 1, 66, 64}, {254, 446, 66, 64}};
                    break;

            default:
                    texture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Spritesheets/spritesheet-enemies-default.png");
                    initAnimation(100, 100, 3);
                    frames= {{128, 213, 64, 44}, {127, 87, 64, 44}, {128, 150, 64, 44}};
                    break;
            }
            break;

        case KingdomTheme::Kingdom2:
            switch (type)
            {
            case EnemyType::Static:
                    texture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Spritesheets/spritesheet-enemies-default.png");
                    initAnimation(100, 100, 3);
                    frames= {{196, 200, 57, 58}, {195, 135, 57, 58}, {193, 79, 62, 49}};
                    break;

            case EnemyType::Patrol:
                    texture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Spritesheets/spritesheet-enemies-default.png");
                    initAnimation(100, 100, 3);
                    frames= {{321, 131, 64, 62}, {320, 194, 64, 62}, {320, 256, 64, 62}};
                    break;

            case EnemyType::Jumping:
                    texture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Spritesheets/spritesheet-enemies-default.png");
                    initAnimation(100, 100, 3);
                    frames= {{320, 70, 64, 59}, {318, 0, 67, 65}, {256, 447, 64, 65}};
                    break;

            default:
                    texture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Spritesheets/spritesheet-enemies-default.png");
                    initAnimation(100, 100, 3);
                    frames= {{196, 200, 57, 58}, {195, 135, 57, 58}, {193, 79, 62, 49}};
                    break;
            }
            break;
    }
    sprite.setTexture(*texture);


}

void Enemy::initAnimation(int frameWidth, int frameHeight, int numFrames){
    frames.clear();
    for (int i = 0; i < numFrames; ++i)
    {
        frames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);
    }
    sprite.setTextureRect(frames[0]);

    // flip horizontally if direction is negative
    sprite.setScale((direction < 0.f) ? -1.f : 1.f, 1.f);
}


void Enemy::update(float dt) {
    if (!alive) return;

    // Animate
    animTimer += dt;
    if (animTimer >= animInterval)
    {
        animTimer = 0.f;
        currentFrame = (currentFrame + 1) % frames.size();
        sprite.setTextureRect(frames[currentFrame]);
    }

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
                vVel = -500.f; //go upward
            }
            vVel += gravity * dt;
            sprite.move(0.f, vVel * dt);

            // Frog is falling
            if (vVel > 0 && jumpState == JumpState::JumpingUp)
                jumpState = JumpState::Falling;

            // Ground detection (simple)
            if (sprite.getPosition().y > startY) {
                sprite.setPosition(sprite.getPosition().x, startY);
                vVel = 0.f;
            }

            // ANIMATION SELECTION
            switch (jumpState) {
                case JumpState::Idle:
                    currentFrame = 0;
                    break;

                case JumpState::JumpingUp:
                    currentFrame = 1;
                    break;

                case JumpState::Falling:
                    currentFrame = 2;
                    break;
            }

            sprite.setTextureRect(frames[currentFrame]);
            break;
        }
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
