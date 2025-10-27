#include <SFML/Graphics.hpp>

int main() {
    // Create a window of size 400x300
    sf::RenderWindow window(sf::VideoMode(400, 300), "Bouncing Ball");

    // Create a ball (circle) with radius 50
    sf::CircleShape ball(20);
    ball.setFillColor(sf::Color::White);
    
    // Initial position of the ball (starting near the center)
    ball.setPosition(100, 100);

    // Ball velocity (initially moving in both X and Y directions)
    sf::Vector2f velocity(0.05f, 0.05f); // X and Y velocities

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Get the current position of the ball
        sf::Vector2f position = ball.getPosition();

        // Check for collision with the walls and bounce back
        if (position.x <= 0 || position.x + ball.getRadius() * 2 >= 400) {
            velocity.x = -velocity.x;  // Reverse direction in X axis
        }
        if (position.y <= 0 || position.y + ball.getRadius() * 2 >= 300) {
            velocity.y = -velocity.y;  // Reverse direction in Y axis
        }

        // Move the ball by the velocity
        ball.move(velocity);

        // Clear the screen
        window.clear();

        // Draw the ball
        window.draw(ball);

        // Display the contents
        window.display();
    }

    return 0;
}
