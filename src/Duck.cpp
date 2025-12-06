#include "Duck.hpp"
#include <cstdlib>
#include <cmath>

Duck::Duck(float x, float y, sf::Texture& texture, float scale, float baseSpeed, DuckType duckType) 
    : alive(true), scale(scale), hitRadius(35.f), type(duckType)
{
    sprite.setTexture(texture);
    
    // Set origin to center of sprite
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    
    // Scale the sprite
    sprite.setScale(scale, scale);
    
    // Set initial position
    sprite.setPosition(x, y);

    // Random velocity based on baseSpeed
    velocity.x = baseSpeed + (rand() % 40) / 100.f;
    velocity.y = 0.f;
}

void Duck::update()
{
    if (!alive)
        return;

    sf::Vector2f pos = sprite.getPosition();
    pos.x += velocity.x;
    pos.y += sin(pos.x * 0.07f) * 1.8f;

    if (pos.y < 120)  // Updated for 600 height - keeps ducks below UI
        pos.y = 120;
    if (pos.y > 480)  // Updated for 600 height - keeps ducks above ground
        pos.y = 480;

    sprite.setPosition(pos);
}

void Duck::draw(sf::RenderWindow &window)
{
    if (alive)
        window.draw(sprite);
}

bool Duck::isHit(const sf::Vector2f &shooter_pos)
{
    if (!alive)
        return false;
    
    float dx = sprite.getPosition().x - shooter_pos.x;
    float dy = sprite.getPosition().y - shooter_pos.y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < hitRadius)
    {
        alive = false;
        return true;
    }
    return false;
}

bool Duck::isOffScreen() const
{
    return sprite.getPosition().x > 850;  // 800 + 50 margin
}

bool Duck::isAlive() const
{
    return alive;
}

DuckType Duck::getType() const
{
    return type;
}