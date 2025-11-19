#include <SFML/Graphics.hpp> //to this we can later add a speaker at the top right side of the screen to allow the user to have the option of keeping the sound on or off while playing 
#include <vector>
#include <iostream>
using namespace std;

// create clouds (three overlapping circles)
static vector<sf::CircleShape> createCloud(float x, float y, float scale = 1.f, sf::Uint8 alpha = 255u) {
    vector<sf::CircleShape> parts;
    sf::Color cloudColor(255, 255, 255, alpha);

    float r1 = 20.f * scale;
    float r2 = 16.f * scale;
    float r3 = 18.f * scale;

    sf::CircleShape c1(r1); c1.setFillColor(cloudColor); c1.setOrigin(r1, r1);
    sf::CircleShape c2(r2); c2.setFillColor(cloudColor); c2.setOrigin(r2, r2);
    sf::CircleShape c3(r3); c3.setFillColor(cloudColor); c3.setOrigin(r3, r3);

    c1.setPosition(x - r2 * 0.6f, y);
    c2.setPosition(x + r2 * 0.4f, y - r2 * 0.15f);
    c3.setPosition(x + r2 * 1.0f, y + r2 * 0.12f);

    parts.push_back(c1);
    parts.push_back(c2);
    parts.push_back(c3);
    return parts;
}

int main() {
    const unsigned int W = 400;
    const unsigned int H = 300;
    sf::RenderWindow window(sf::VideoMode(W, H), "Duck Shooting", sf::Style::Close);
    window.setFramerateLimit(60);

    // Colors
    sf::Color skyColor(135, 206, 235);
    sf::Color groundColor(139, 69, 19);
    sf::Color grassColor(34, 139, 34);

    // Ground and grass
    float groundHeight = 60.f;
    float grassHeight = 12.f;
    sf::RectangleShape ground(sf::Vector2f((float)W, groundHeight));
    ground.setFillColor(groundColor);
    ground.setPosition(0.f, (float)H - groundHeight);

    sf::RectangleShape grass(sf::Vector2f((float)W, grassHeight));
    grass.setFillColor(grassColor);
    grass.setPosition(0.f, (float)H - groundHeight - grassHeight + 3.f);

    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        return -1;
    }

    // Title text
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("DUCK SHOOTING");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(2.f);

    sf::FloatRect tbTitle = titleText.getLocalBounds();
    titleText.setOrigin(tbTitle.left + tbTitle.width / 2.f, tbTitle.top + tbTitle.height / 2.f);
    titleText.setPosition(W / 2.f, (H / 2.f) - 50.f);

    float paddingOuter = 14.f;
    float paddingInner = 8.f;

    sf::RectangleShape outerBox(sf::Vector2f(tbTitle.width + paddingOuter * 2.f, tbTitle.height + paddingOuter * 2.f));
    outerBox.setFillColor(sf::Color(0, 0, 128)); // dark blue
    outerBox.setOrigin(outerBox.getSize().x / 2.f, outerBox.getSize().y / 2.f);
    outerBox.setPosition(titleText.getPosition());

    sf::RectangleShape tealBorder(sf::Vector2f(tbTitle.width + paddingInner * 2.f, tbTitle.height + paddingInner * 2.f));
    tealBorder.setFillColor(sf::Color::Transparent);
    tealBorder.setOutlineColor(sf::Color(0, 128, 128)); // teal colour
    tealBorder.setOutlineThickness(4.f);
    tealBorder.setOrigin(tealBorder.getSize().x / 2.f, tealBorder.getSize().y / 2.f);
    tealBorder.setPosition(titleText.getPosition());

    // Start text
    sf::Text startText;
    startText.setFont(font);
    startText.setString("PRESS ENTER TO START");
    startText.setCharacterSize(20);
    startText.setFillColor(sf::Color::White);
    startText.setOutlineColor(sf::Color(0, 0, 0, 200));
    startText.setOutlineThickness(2.f);

    sf::FloatRect tbStart = startText.getLocalBounds();
    startText.setOrigin(tbStart.left + tbStart.width / 2.f, tbStart.top + tbStart.height / 2.f);
    // Move 3 steps (≈30px) below the grass-ground boundary
    startText.setPosition(W / 2.f, H - groundHeight - (grassHeight / 2.f) + 10.f);

    // Clouds
    vector<vector<sf::CircleShape>> clouds;
    clouds.push_back(createCloud(60.f, 45.f, 1.1f, 255));
    clouds.push_back(createCloud(170.f, 35.f, 0.6f, 200));
    clouds.push_back(createCloud(270.f, 50.f, 1.0f, 255));
    clouds.push_back(createCloud(350.f, 30.f, 0.55f, 190));

    bool started = false;

    while (window.isOpen() && !started) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) started = true;
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }

        window.clear(skyColor);

        for (const auto &parts : clouds)
            for (const auto &c : parts)
                window.draw(c);

        window.draw(ground);
        window.draw(grass);

        // Draw bordered title
        window.draw(outerBox);
        window.draw(tealBorder);
        window.draw(titleText);

        // Draw start text
        window.draw(startText);

        window.display();
    }

    return 0;
}
