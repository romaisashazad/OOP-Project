#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Drawing a Circle");

    // Create a circle shape with radius 50
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);

    // Set the position of the circle in the center of the window
    circle.setPosition(400 - 50, 300 - 50); // (window width/2 - radius, window height/2 - radius)

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(circle); // Draw the circle on the window
        window.display();    // Display the contents
    }

    return 0;
}
