//
// Created by prais on 04/12/2025.
//

#ifndef THE_QUEST_PROJECTILE_HPP
#define THE_QUEST_PROJECTILE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Enemy.hpp"

class Projectile {
public:
    Projectile(float x, float y, float dir, sf::Texture* tex);

    void update(float dt);
    void render(sf::RenderWindow& w);
    sf::FloatRect getBounds() const;   // for collision checks

    bool isActive() const {return active;}
    void destroy() {active = false;}

    bool alive = true;

private:
    sf::Sprite sprite;
    float speed = 600.f;
    float direction = 1.f;
    bool active = true;
};



#endif //THE_QUEST_PROJECTILE_HPP