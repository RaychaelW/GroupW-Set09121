//
// Created by prais on 21/11/2025.
//

#ifndef THE_QUEST_PLAYER_HPP
#define THE_QUEST_PLAYER_HPP


#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.hpp"
#include "ResourceManager.hpp"
#include "Tilemap.hpp"

struct Frame {
    int x, y, width, height;
};

enum class MoveType {
    Idle,
    Walk,
    Jump,
    Hit
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
    void update(float dt);
    void render(sf::RenderWindow& window);
    void setCharacter(CharacterColor color);

    sf::Sprite& getSprite() {return sprite;}

    sf::Vector2f getPosition() const {return sprite.getPosition();}
    void setPosition(const sf::Vector2f& pos) {sprite.setPosition(pos);}

    sf::Vector2f& getVelocity() { return velocity; }
    sf::FloatRect getBounds() const {return sprite.getGlobalBounds();}
    void setVelocityY(float vy) {velocity.y = vy;}
    void setIsGrounded(bool grounded) {isGrounded = grounded;}

    //player stats
    int getLives() const {return lives;}
    void damage();
    void loseLife();
    int addCoin();
    int getCoins() const{ return coins;}
    bool dead() const { return isDead;}

    //platform logic
    float verticalVelocity() const { return velocity.y;}
    void setOnGround(bool grounded) { isGrounded = grounded;}
    void stopVertical() { velocity.y = 0.f;}


private:
    InputManager input;
    sf::Sprite sprite;
    sf::Texture *texture;
    sf::Vector2f position;
    bool facingRight = true;

    TileMap map;

    //movement
    sf::Vector2f velocity;
    float speed = 200.f;
    float jumpForce = 600.f;
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

    //health and coins
    int lives = 3;
    bool isInvincible = false;
    float invincibleTimer = 0.f;
    bool isDead = false;
    int coins = 0;
};


#endif //THE_QUEST_PLAYER_HPP