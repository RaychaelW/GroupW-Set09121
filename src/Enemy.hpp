#ifndef THE_QUEST_ENEMY_HPP
#define THE_QUEST_ENEMY_HPP

#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(float x, float y);

    void update(float dt);       // optional, e.g., animations
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Sprite& getSprite() { return sprite; }

private:
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif //THE_QUEST_ENEMY_HPP
