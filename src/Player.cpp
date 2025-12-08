#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include <iostream>
#include <tmxlite/Map.hpp>
#include "Projectile.hpp"
#include "Tilemap.hpp"
#include "SoundManager.hpp"
#include "GlobalSettings.hpp"

// Helper function for Player class
void playSoundWithPlayerSettings(sf::Sound& sound, float baseVolume) {
    auto& settings = GlobalSettings::getInstance();

    if (!settings.isSFXOn()) {
        return; // Don't play sound if SFX is off
    }

    float actualVolume = baseVolume * (settings.getMasterVolume() / 100.0f);
    sound.setVolume(actualVolume);
    sound.play();
}

Player::Player() {
    texture = ResourceManager::getInstance().getTexture("resources/tilesets/Sprites/Spritesheets/spritesheet-characters-default.png");
    sprite.setTexture(*texture);
    sprite.setPosition(100.f, 300.f);
    velocity = sf::Vector2f(0.f, 0.f);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

    int frameWidth = 100;
    int frameHeight = 100;

    //default beige character
    frames[CharacterColor::Beige][MoveType::Idle] = {
        {525, 924, frameWidth, frameHeight} // 1 idle frames
    };
    frames[CharacterColor::Beige][MoveType::Walk] = {
        {526, 539, frameWidth, frameHeight}, {525, 667, frameWidth, frameHeight} // 2 walk frames
    };
    frames[CharacterColor::Beige][MoveType::Jump] = {
        {527, 795, frameWidth, frameHeight} // 1 jump frame
    };
    frames[CharacterColor::Beige][MoveType::Hit] = {
        {653, 29, frameWidth, frameHeight} // 1 jump frame
    };
}

void Player::updateAnimation(float dt) {
    animationTimer += dt;
    if (animationTimer >= animationSpeed) {
        animationTimer = 0.f;
        currentFrame++;

        const auto& moveFrames = frames[currentColor][currentMove];
        if (!moveFrames.empty()) {
            currentFrame %= moveFrames.size();
            Frame f = moveFrames[currentFrame];
            sprite.setTextureRect(sf::IntRect(f.x, f.y, f.width, f.height));
        }

        //turn around when walking
        sprite.setScale(facingRight ? 1.f : -1.f, 1.f);
        if (!facingRight)
            sprite.setOrigin(128, 0);
        else
            sprite.setOrigin(0, 0);
    }
}


void Player::handleInput() {
    // horizontal movement
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= speed;
        currentMove = MoveType::Walk;
        facingRight = false;
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += speed;
        currentMove = MoveType::Walk;
        facingRight = true;
    }
    else {
        currentMove = MoveType::Idle;
    }

    //jump
    static bool spaceWasPressed = false;
    bool spaceIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    
    if (spaceIsPressed && !spaceWasPressed && isGrounded) {
        velocity.y = -jumpForce;
        isGrounded = false;
        currentMove = MoveType::Jump;

        
        spaceWasPressed = true;
    }
    
    spaceWasPressed = spaceIsPressed;

    if (!isGrounded) {
        currentMove = MoveType::Jump;
    }
}


void Player::update(float dt) {

    //update invincibility
    if (isInvincible) {
        invincibleTimer -= dt;
        if (invincibleTimer <= 0) {
            isInvincible = false;
        }
    }

    //apply gravity
    velocity.y += gravity * dt;
    sprite.move(velocity * dt);

    const sf::FloatRect bounds = sprite.getGlobalBounds(); //get player bounds
    // Top collision
    if (bounds.top < 0) {
        velocity.y = 0.f;
    }
    //left wall collision
    if (bounds.left < 0) {
        velocity.x = 0.f;
    }
    /* doesnt work for now (right)
    if (bounds.left + bounds.width > window.getSize().x) {
        velocity.x = 0.f;
    }
    */

    if (isHit) {
        hitTimer -= dt;
        velocity = knockBackVelocity;

        if (hitTimer <= 0) {
            isHit = false;
            currentMove = MoveType::Idle;
        }
    }

    updateAnimation(dt); //animate

}

int Player::addCoin() {
    coins++;

    
    std::cout << "Coins: " << coins << "\n";
    return coins;
}



void Player::damage() {
    sf::IntRect hitFrame = sf::IntRect(653, 16, 100, 100);

    if (isInvincible || isDead) return;

    if (isHit) return;
    lives -= 1;
    isHit = true;
    hitTimer = hitDuration;
    
    //bounce back after hit
    knockBackVelocity = sf::Vector2f( facingRight ? -150.f : 150.f, -120.f);
    sprite.setTextureRect(hitFrame);


    if (lives < 0) lives = 0;
    std::cout << "Player hit! Lives left: " << lives << "\n";


    if (lives == 0) {
        isDead = true;
        return;
    }

    //hit animation
    if (!isDead) {
        currentMove = MoveType::Hit;
        sprite.setTextureRect(hitFrame);
        isInvincible = true;
        invincibleTimer = 0.75f; //second of immunity
    }

}

void Player::reset() {
    lives = 3;
    coins = 0;
    velocity = {0,0};

    setPosition({100.f, 100.f});
}


void Player::loseLife() {
    if (lives > 0)
        lives--;

    if (lives == 0)
        isDead = true;
}


void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
}