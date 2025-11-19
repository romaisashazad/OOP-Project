#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

using namespace std;

// ==================== CLOUD FUNCTION ====================
static vector<sf::CircleShape> createCloud(float x, float y, float scale = 1.f, sf::Uint8 alpha = 255u)
{
    vector<sf::CircleShape> parts;
    sf::Color cloudColor(255, 255, 255, alpha);

    float r1 = 20.f * scale;
    float r2 = 16.f * scale;
    float r3 = 18.f * scale;

    sf::CircleShape c1(r1);
    c1.setFillColor(cloudColor);
    c1.setOrigin(r1, r1);
    sf::CircleShape c2(r2);
    c2.setFillColor(cloudColor);
    c2.setOrigin(r2, r2);
    sf::CircleShape c3(r3);
    c3.setFillColor(cloudColor);
    c3.setOrigin(r3, r3);

    c1.setPosition(x - r2 * 0.6f, y);
    c2.setPosition(x + r2 * 0.4f, y - r2 * 0.15f);
    c3.setPosition(x + r2 * 1.0f, y + r2 * 0.12f);

    parts.push_back(c1);
    parts.push_back(c2);
    parts.push_back(c3);
    return parts;
}

// ==================== DUCK CLASS ====================
class Duck
{
private:
    sf::CircleShape body;
    sf::Vector2f velocity;
    bool alive;

public:
    Duck(float x, float y, float scale = 1.0f) : alive(true)
    {
        body.setRadius(16.f * scale);
        body.setFillColor(sf::Color(255, 255, 0)); // Bright yellow
        body.setOutlineColor(sf::Color(255, 200, 0));
        body.setOutlineThickness(2.f);
        body.setOrigin(16.f * scale, 16.f * scale);
        body.setPosition(x, y);

        velocity.x = 1.2f + (rand() % 40) / 100.f; // Slight random speed
        velocity.y = 0.f;
    }

    void update()
    {
        if (!alive)
            return;

        sf::Vector2f pos = body.getPosition();
        pos.x += velocity.x;

        // Beautiful wavy flying motion
        pos.y += sin(pos.x * 0.07f) * 1.8f;

        // Keep in sky area
        if (pos.y < 70)
            pos.y = 70;
        if (pos.y > 230)
            pos.y = 230;

        body.setPosition(pos);
    }

    void draw(sf::RenderWindow &window)
    {
        if (alive)
            window.draw(body);
    }

    bool isHit(const sf::Vector2f &shooterPos)
    {
        if (!alive)
            return false;

        float dx = body.getPosition().x - shooterPos.x;
        float dy = body.getPosition().y - shooterPos.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < 24.f) // Slightly larger hitbox = fair
        {
            alive = false;
            return true;
        }
        return false;
    }

    bool isOffScreen() const
    {
        return body.getPosition().x > 460;
    }

    bool isAlive() const { return alive; }
};

// ==================== SHOOTER CLASS ====================
class Shooter
{
private:
    sf::Vector2f position;

public:
    Shooter() : position(200, 150) {}

    void move(float dx, float dy)
    {
        position.x += dx;
        position.y += dy;
        position.x = max(30.f, min(370.f, position.x));
        position.y = max(60.f, min(240.f, position.y));
    }

    void draw(sf::RenderWindow &window)
    {
        sf::Vector2f pos = position;

        // Glowing outer ring
        sf::CircleShape glow(34);
        glow.setFillColor(sf::Color::Transparent);
        glow.setOutlineColor(sf::Color(0, 255, 255, 90));
        glow.setOutlineThickness(8);
        glow.setOrigin(34, 34);
        glow.setPosition(pos);
        window.draw(glow);

        // Bright cyan cross
        float size = 28.f;
        sf::Vertex line1[] = {
            sf::Vertex(sf::Vector2f(pos.x - size, pos.y - size), sf::Color::Cyan),
            sf::Vertex(sf::Vector2f(pos.x + size, pos.y + size), sf::Color::Cyan)};
        sf::Vertex line2[] = {
            sf::Vertex(sf::Vector2f(pos.x - size, pos.y + size), sf::Color::Cyan),
            sf::Vertex(sf::Vector2f(pos.x + size, pos.y - size), sf::Color::Cyan)};
        window.draw(line1, 2, sf::Lines);
        window.draw(line2, 2, sf::Lines);

        // Center dot
        sf::CircleShape dot(5);
        dot.setFillColor(sf::Color::White);
        dot.setOrigin(5, 5);
        dot.setPosition(pos);
        window.draw(dot);
    }

    sf::Vector2f getPosition() const { return position; }
};

// ==================== GAME STATES ====================
enum GameState
{
    WELCOME,
    INSTRUCTIONS,
    PLAYING,
    GAME_OVER
};

int main()
{
    const unsigned int W = 400;
    const unsigned int H = 300;

    sf::RenderWindow window(sf::VideoMode(W, H), "Duck Shooting Game", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    srand(time(nullptr));

    // Background
    sf::Color skyColor(135, 206, 235);
    sf::Color groundColor(139, 69, 19);
    sf::Color grassColor(34, 139, 34);

    sf::RectangleShape ground(sf::Vector2f(W, 60));
    ground.setFillColor(groundColor);
    ground.setPosition(0, H - 60);

    sf::RectangleShape grass(sf::Vector2f(W, 12));
    grass.setFillColor(grassColor);
    grass.setPosition(0, H - 60 - 12 + 3);

    vector<vector<sf::CircleShape>> clouds;
    clouds.push_back(createCloud(60, 45, 1.1f));
    clouds.push_back(createCloud(170, 35, 0.6f, 200));
    clouds.push_back(createCloud(270, 50, 1.0f));
    clouds.push_back(createCloud(350, 30, 0.55f, 190));

    // Font
    sf::Font font;
    bool fontLoaded = font.loadFromFile("Arial.ttf");
    if (!fontLoaded)
    {
        cout << "Arial.ttf not found — text will be missing, but game still works!\n";
    }

    GameState state = WELCOME;
    int score = 0, missed = 0;

    Shooter *shooter = nullptr;
    vector<Duck> *ducks = nullptr;
    sf::Clock *spawnClock = nullptr;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (state == WELCOME)
                {
                    if (event.key.code == sf::Keyboard::Enter)
                        state = PLAYING;
                    if (event.key.code == sf::Keyboard::I)
                        state = INSTRUCTIONS;
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                }
                else if (state == INSTRUCTIONS)
                {
                    if (event.key.code == sf::Keyboard::Enter)
                        state = PLAYING;
                    if (event.key.code == sf::Keyboard::Escape)
                        state = WELCOME;
                }
                else if (state == PLAYING && event.key.code == sf::Keyboard::Escape)
                {
                    state = WELCOME;
                    delete shooter;
                    delete ducks;
                    delete spawnClock;
                    shooter = nullptr;
                    ducks = nullptr;
                    spawnClock = nullptr;
                }
                else if (state == GAME_OVER)
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        state = WELCOME;
                        score = 0;
                        missed = 0;
                    }
                }
            }
        }

        // =============== UPDATE ===============
        if (state == PLAYING)
        {
            if (!shooter)
            {
                shooter = new Shooter();
                ducks = new vector<Duck>();
                spawnClock = new sf::Clock();
                score = 0;
                missed = 0;
            }

            // Move shooter
            float speed = 4.5f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                shooter->move(-speed, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                shooter->move(speed, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                shooter->move(0, -speed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                shooter->move(0, speed);

            // Spawn ducks
            if (spawnClock->getElapsedTime().asSeconds() > 2.8f)
            {
                float y = 90 + rand() % 120;
                ducks->emplace_back(-40, y, 1.0f);
                spawnClock->restart();
            }

            // Update ducks & check hits
            for (auto &duck : *ducks)
            {
                duck.update();

                if (duck.isHit(shooter->getPosition()))
                {
                    score += 10;
                    window.clear(sf::Color::White);
                    window.display();
                    sf::sleep(sf::milliseconds(90));
                }

                if (duck.isOffScreen() && duck.isAlive())
                    missed++;
            }

            // Remove dead/off-screen ducks
            ducks->erase(remove_if(ducks->begin(), ducks->end(),
                                   [](const Duck &d)
                                   { return !d.isAlive() || d.isOffScreen(); }),
                         ducks->end());

            if (missed >= 3)
            {
                state = GAME_OVER;
                delete shooter;
                delete ducks;
                delete spawnClock;
                shooter = nullptr;
                ducks = nullptr;
                spawnClock = nullptr;
            }
        }

        // =============== DRAW ===============
        window.clear(skyColor);

        // Background
        for (const auto &cloud : clouds)
            for (const auto &c : cloud)
                window.draw(c);
        window.draw(ground);
        window.draw(grass);

        // Text (only if font loaded)
        if (fontLoaded)
        {
            if (state == WELCOME)
            {
                sf::Text title("DUCK SHOOTING GAME", font, 34);
                title.setFillColor(sf::Color::Yellow);
                title.setStyle(sf::Text::Bold);
                title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
                title.setPosition(W / 2.f, 80);
                window.draw(title);

                sf::Text start("PRESS ENTER TO PLAY", font, 22);
                start.setFillColor(sf::Color::White);
                start.setOrigin(start.getLocalBounds().width / 2, start.getLocalBounds().height / 2);
                start.setPosition(W / 2.f, 150);
                window.draw(start);

                sf::Text inst("PRESS I FOR INSTRUCTIONS", font, 18);
                inst.setFillColor(sf::Color(200, 200, 255));
                inst.setOrigin(inst.getLocalBounds().width / 2, inst.getLocalBounds().height / 2);
                inst.setPosition(W / 2.f, 180);
                window.draw(inst);
            }
            else if (state == INSTRUCTIONS)
            {
                sf::Text text;
                text.setFont(font);
                text.setCharacterSize(18);
                text.setFillColor(sf::Color::White);
                text.setPosition(20, 50);
                text.setString(
                    "HOW TO PLAY:\n\n"
                    "Use ARROW KEYS to move the cyan shooter\n"
                    "Touch the yellow ducks to shoot them!\n"
                    "+10 points per duck\n"
                    "Let 3 ducks escape = Game Over\n\n"
                    "Press ENTER to start\n"
                    "Press ESC anytime to return to menu");
                window.draw(text);
            }
            else if (state == PLAYING)
            {
                sf::Text hud("Score: " + to_string(score) + "   Missed: " + to_string(missed), font, 20);
                hud.setFillColor(sf::Color::White);
                hud.setPosition(10, 8);
                window.draw(hud);
            }
            else if (state == GAME_OVER)
            {
                sf::Text go("GAME OVER", font, 44);
                go.setFillColor(sf::Color::Red);
                go.setStyle(sf::Text::Bold);
                go.setOrigin(go.getLocalBounds().width / 2, go.getLocalBounds().height / 2);
                go.setPosition(W / 2.f, 90);
                window.draw(go);

                sf::Text final("FINAL SCORE: " + to_string(score), font, 28);
                final.setFillColor(sf::Color::Yellow);
                final.setOrigin(final.getLocalBounds().width / 2, final.getLocalBounds().height / 2);
                final.setPosition(W / 2.f, 140);
                window.draw(final);

                sf::Text retry("PRESS ENTER TO TRY AGAIN", font, 20);
                retry.setFillColor(sf::Color::White);
                retry.setOrigin(retry.getLocalBounds().width / 2, retry.getLocalBounds().height / 2);
                retry.setPosition(W / 2.f, 200);
                window.draw(retry);
            }
        }

        // Draw game objects
        if (state == PLAYING && shooter && ducks)
        {
            for (auto &duck : *ducks)
                duck.draw(window);
            shooter->draw(window);
        }

        window.display();
    }

    // Cleanup
    delete shooter;
    delete ducks;
    delete spawnClock;

    return 0;
}