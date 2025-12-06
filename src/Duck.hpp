#ifndef DUCK_HPP
#define DUCK_HPP
#include <SFML/Graphics.hpp>

enum DuckType{ //duck types for difficulty progression
    YELLOW, //good duck -> +10 points if shot
    GREEN, //bad duck, to be avoided -> -15 points if shot
    BROWN //bad duck, to be avoided -> -15 points if shot
};
class Duck{
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    bool alive;
    float scale;
    float hitRadius;
    DuckType type; //behaviour to be based on duck colour

public:
    Duck(float x, float y, sf::Texture& texture, float scale, float baseSpeed, DuckType duckType);
    void update();
    void draw(sf::RenderWindow &window);
    bool isHit(const sf::Vector2f &shooter_pos); //checks if the distance is less than the hitradius then duck has been shot 
    bool isOffScreen() const;
    bool isAlive() const;
    DuckType getType() const;

};
#endif
