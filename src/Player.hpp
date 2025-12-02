//
// Created by prais on 21/11/2025.
//

#ifndef THE_QUEST_PLAYER_HPP
#define THE_QUEST_PLAYER_HPP


#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.hpp"
#include "ResourceManager.hpp"

struct Frame {
    int x, y, width, height;
};

enum class MoveType {
    Idle,
    Walk,
    Jump
};

enum class CharacterColor {
    Beige,
    Purple,
    Pink,
    Green,
    Yellow
};

class Player {
public:
    Player();

    void handleInput();
    void update(float dt, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    void setCharacter(CharacterColor color);

    sf::Sprite& getSprite() {
        return sprite;
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
    }


private:
    InputManager input;
    sf::Sprite sprite;
    sf::Texture *texture;
    sf::Vector2f position;
    bool facingRight = true;


    //movement
    sf::Vector2f velocity;
    float speed = 200.f;
    float jumpForce = 350.f;
    float isGrounded = false;
    float gravity = 981.f;

    CharacterColor currentColor = CharacterColor::Beige;

    //animation
    std::map<CharacterColor, std::map<MoveType, std::vector<Frame>>> frames;
    MoveType currentMove = MoveType::Idle;
    size_t currentFrame = 0;
    float animationTimer = 0.f; //switching frames
    float animationSpeed = 0.2f; //secs per frame


    void updateAnimation(float dt); //animation helper

};


#endif //THE_QUEST_PLAYER_HPP