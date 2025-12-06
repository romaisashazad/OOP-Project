#ifndef SHOOTER_HPP
#define SHOOTER_HPP
#include <SFML/Graphics.hpp>

class Shooter {
private:
    sf::Vector2f position;

public:
    Shooter();
    void move(float dx, float dy);
    void draw(sf::RenderWindow &window);
    sf::Vector2f getPosition() const;
};

#endif