#include "Shooter.hpp"
#include <algorithm>

Shooter::Shooter() : position(400, 300){}

void Shooter::move(float dx, float dy){
    position.x += dx;
    position.y += dy;
    position.x = std::max(50.f, std::min(750.f, position.x));
    position.y = std::max(100.f, std::min(460.f, position.y)); 
}

void Shooter::draw(sf::RenderWindow &window){
    sf::Vector2f pos = position;
    sf::CircleShape glow(34);
    glow.setFillColor(sf::Color::Transparent);
    glow.setOutlineColor(sf::Color(0, 255, 255, 90));
    glow.setOutlineThickness(8);
    glow.setOrigin(34, 34);
    glow.setPosition(pos);
    window.draw(glow);

    float size = 28.f;
    sf::Vertex line1[] = {
        sf::Vertex(sf::Vector2f(pos.x - size, pos.y - size), sf::Color::Black),
        sf::Vertex(sf::Vector2f(pos.x + size, pos.y + size), sf::Color::Black)
    };
    sf::Vertex line2[] = {
        sf::Vertex(sf::Vector2f(pos.x - size, pos.y + size), sf::Color::Black),
        sf::Vertex(sf::Vector2f(pos.x + size, pos.y - size), sf::Color::Black)
    };
    window.draw(line1, 2, sf::Lines);
    window.draw(line2, 2, sf::Lines);

    sf::CircleShape dot(5);
    dot.setFillColor(sf::Color::White);
    dot.setOrigin(5, 5);
    dot.setPosition(pos);
    window.draw(dot);
}

sf::Vector2f Shooter::getPosition() const{
    return position;
}