#include <SFML/Graphics.hpp>
#include <vector>
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

// Function to draw one duck at a given position
static void drawDuck(sf::RenderWindow &window, float x, float y, float scale = 1.0f) {
    sf::Color yellow(255, 255, 0);
    sf::Color orange(255, 165, 0);

    // Body
    sf::CircleShape body(14.f * scale);
    body.setFillColor(yellow);
    body.setOrigin(14.f * scale, 14.f * scale);
    body.setPosition(x, y);

    // Head
    sf::CircleShape head(8.f * scale);
    head.setFillColor(yellow);
    head.setOrigin(8.f * scale, 8.f * scale);
    head.setPosition(x + 12.f * scale, y - 10.f * scale);

    // Beak (triangle)
    sf::ConvexShape beak;
    beak.setPointCount(3);
    beak.setPoint(0, sf::Vector2f(0.f, 0.f));
    beak.setPoint(1, sf::Vector2f(8.f * scale, 4.f * scale));
    beak.setPoint(2, sf::Vector2f(0.f, 8.f * scale));
    beak.setFillColor(orange);
    beak.setPosition(x + 20.f * scale, y - 10.f * scale);

    // Wings
    sf::CircleShape wingLeft(6.f * scale);
    wingLeft.setFillColor(sf::Color(255, 220, 0));
    wingLeft.setOrigin(6.f * scale, 6.f * scale);
    wingLeft.setPosition(x - 10.f * scale, y - 4.f * scale);

    sf::CircleShape wingRight(6.f * scale);
    wingRight.setFillColor(sf::Color(255, 220, 0));
    wingRight.setOrigin(6.f * scale, 6.f * scale);
    wingRight.setPosition(x + 10.f * scale, y - 4.f * scale);

    window.draw(body);
    window.draw(wingLeft);
    window.draw(wingRight);
    window.draw(head);
    window.draw(beak);
}

// Function to draw an X shooter scaled relative to duck size
static void drawShooter(sf::RenderWindow &window, float x, float y, float duckScale) {
    float baseSize = 14.f * duckScale;     // base duck size
    float shooterSize = baseSize * 1.5f;   // 1.5 times duck size

    sf::Vector2f center(x, y);

    sf::Vertex line1[] = {
        sf::Vertex(sf::Vector2f(center.x - shooterSize / 2, center.y - shooterSize / 2), sf::Color::White),
        sf::Vertex(sf::Vector2f(center.x + shooterSize / 2, center.y + shooterSize / 2), sf::Color::White)
    };
    sf::Vertex line2[] = {
        sf::Vertex(sf::Vector2f(center.x - shooterSize / 2, center.y + shooterSize / 2), sf::Color::White),
        sf::Vertex(sf::Vector2f(center.x + shooterSize / 2, center.y - shooterSize / 2), sf::Color::White)
    };

    window.draw(line1, 2, sf::Lines);
    window.draw(line2, 2, sf::Lines);
}

int main() {
    const unsigned int W = 400;
    const unsigned int H = 300;
    sf::RenderWindow window(sf::VideoMode(W, H), "Duck Shooting", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Color skyColor(135, 206, 235);
    sf::Color groundColor(139, 69, 19);
    sf::Color grassColor(34, 139, 34);

    float groundHeight = 60.f;
    float grassHeight = 12.f;

    sf::RectangleShape ground(sf::Vector2f((float)W, groundHeight));
    ground.setFillColor(groundColor);
    ground.setPosition(0.f, H - groundHeight);

    sf::RectangleShape grass(sf::Vector2f((float)W, grassHeight));
    grass.setFillColor(grassColor);
    grass.setPosition(0.f, H - groundHeight - grassHeight + 3.f);

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

        // Draw ducks
        float duckScale = 0.9f;
        drawDuck(window, 100.f, 120.f, duckScale);   
        drawDuck(window, 250.f, 90.f, duckScale);    
        drawDuck(window, 320.f, H - groundHeight - 20.f, duckScale);

        // Draw scaled X shooter beside first duck
        drawShooter(window, 150.f, 115.f, duckScale);

        window.display();
    }

    return 0;
}

