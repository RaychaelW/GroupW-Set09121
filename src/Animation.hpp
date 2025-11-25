//
// Created by Tanatswa Mlandeli on 25/11/2025.
//

#ifndef THE_QUEST_ANIMATION_HPP
#define THE_QUEST_ANIMATION_HPP

#include <SFML/Graphics.hpp>

class Animation {
public:
    Animation();

    // Setup:
    //  - spriteSheet = the texture containing all frames
    //  - gridSize = {columns, rows}
    //  - frameTime = seconds per frame
    //  - rowIndex = which row the animation is on (0 = top row)
    void setup(const sf::Texture& spriteSheet,
               sf::Vector2u gridSize,
               float frameTime,
               unsigned rowIndex);

    // Update frame based on dt
    void update(float dt);

    // Returns the current frame rectangle
    sf::IntRect getFrame() const { return currentFrame; }

private:
    const sf::Texture* texture;
    sf::IntRect currentFrame;

    sf::Vector2u gridSize;   // number of frames horizontally/vertically
    float frameTime;         // time per frame
    float timer;             // accumulates dt
    unsigned currentColumn;  // which frame in the row
    unsigned rowIndex;       // which row
};

#endif //THE_QUEST_ANIMATION_HPP
