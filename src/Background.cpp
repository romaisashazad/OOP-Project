
#include "Background.hpp"

std::vector<sf::CircleShape> Background::createCloud(float x, float y, float scale, unsigned char brightness)
{
    std::vector<sf::CircleShape> cloud;
    sf::Color cloudColor(brightness, brightness, brightness, 180);

    for (int i = 0; i < 3; i++)
    {
        sf::CircleShape circle(12 * scale);
        circle.setFillColor(cloudColor);
        circle.setPosition(x + i * 18 * scale, y);
        cloud.push_back(circle);
    }
    return cloud;
}

Background::Background(unsigned int W, unsigned int H)
    : skyColor(135, 206, 235),
      groundColor(139, 69, 19),
      grassColor(34, 139, 34)
{
    ground.setSize(sf::Vector2f(W, 120)); // Larger ground for bigger window
    ground.setFillColor(groundColor);
    ground.setPosition(0, H - 120);

    grass.setSize(sf::Vector2f(W, 24)); // Larger grass
    grass.setFillColor(grassColor);
    grass.setPosition(0, H - 120 - 24 + 6);

    // Add more clouds for wider window (800px)
    clouds.push_back(createCloud(100, 80, 1.1f));
    clouds.push_back(createCloud(300, 60, 0.6f, 200));
    clouds.push_back(createCloud(500, 90, 1.0f));
    clouds.push_back(createCloud(650, 50, 0.55f, 190));
    clouds.push_back(createCloud(200, 120, 0.8f, 210));
}

void Background::render(sf::RenderWindow &window)
{
    window.clear(skyColor);

    for (const std::vector<sf::CircleShape> &cloud : clouds)
    {
        for (const sf::CircleShape &c : cloud)
        {
            window.draw(c);
        }
    }
    window.draw(ground);
    window.draw(grass);
}