
#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;

class Background
{
private:
    sf::Color skyColor;
    sf::Color groundColor;
    sf::Color grassColor;
    sf::RectangleShape ground;
    sf::RectangleShape grass;
    vector<std::vector<sf::CircleShape> > clouds;
    
    vector<sf::CircleShape> createCloud(float x, float y, float scale, unsigned char brightness = 255);

public:
    Background(unsigned int W, unsigned int H);
    void render(sf::RenderWindow& window);
};

#endif