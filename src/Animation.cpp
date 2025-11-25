//
// Created by Tanatswa Mlandeli on 25/11/2025.
//
#include "Animation.hpp"

Animation::Animation()
    : texture(nullptr),
      gridSize(1, 1),
      frameTime(0.1f),
      timer(0.f),
      currentColumn(0),
      rowIndex(0)
{
    currentFrame = sf::IntRect(0, 0, 0, 0);
}

void Animation::setup(const sf::Texture& spriteSheet,
                      sf::Vector2u gridSize,
                      float frameTime,
                      unsigned rowIndex)
{
    this->texture = &spriteSheet;
    this->gridSize = gridSize;
    this->frameTime = frameTime;
    this->rowIndex = rowIndex;
    this->timer = 0.f;
    this->currentColumn = 0;

    // Calculate each frame size
    unsigned frameWidth  = texture->getSize().x / gridSize.x;
    unsigned frameHeight = texture->getSize().y / gridSize.y;

    currentFrame = sf::IntRect(
        0,
        rowIndex * frameHeight,
        frameWidth,
        frameHeight
    );
}

void Animation::update(float dt) {
    if (!texture) return;

    timer += dt;

    // Advance frame when time is reached
    if (timer >= frameTime) {
        timer = 0.f;
        currentColumn++;

        // loop animation
        if (currentColumn >= gridSize.x)
            currentColumn = 0;
    }

    unsigned frameWidth  = texture->getSize().x / gridSize.x;
    unsigned frameHeight = texture->getSize().y / gridSize.y;

    currentFrame.left = currentColumn * frameWidth;
    currentFrame.top  = rowIndex * frameHeight;
    currentFrame.width = frameWidth;
    currentFrame.height = frameHeight;
}
