#include "Projectile.hpp"

Projectile::Projectile(float x, float y, float dir, sf::Texture* tex)
    : direction(dir)
{
    if (tex) {
        sprite.setTexture(*tex);
    }

    // set origin to centre so flipping visually displace it
    sf::FloatRect b = sprite.getLocalBounds();
    sprite.setOrigin(b.width * 0.5f, b.height * 0.5f);
    sprite.setRotation(-90.f); // make horizontal

    sprite.setPosition(x, y); //place in front of player

    // flip horizontally if direction is negative
    sprite.setScale((direction < 0.f) ? -1.f : 1.f, 1.f);
}

void Projectile::update(float dt)
{
    if (!active) return;
    // simple horizontal motion
    sprite.move(direction * speed * dt, 0.f);

    // optionally mark dead if off-screen (tune range as needed)
    float sx = sprite.getPosition().x;
    if (sx < -200.f || sx > 10000.f) alive = false;
}

void Projectile::render(sf::RenderWindow& w)
{
    if (active)
        w.draw(sprite);
}

sf::FloatRect Projectile::getBounds() const
{
    return sprite.getGlobalBounds();
}
