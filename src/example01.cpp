#include <SFML/Graphics.hpp>
#include <iostream>
//This program creates a basic window and closes it on pressing the 'Esc' key

int main() {
    // Create a window with a title and size 800x600
    sf::RenderWindow window(sf::VideoMode(800, 600), "Basic SFML Window");

    // Main loop to keep the window open
    while (window.isOpen()) {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close the window when close button or escape key is pressed
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed 
                 && event.key.code == sf::Keyboard::Escape)) {
                std::cout << "Closed the window";
                window.close();
            }
        }
        // Clear the window with a black color
        window.clear(sf::Color::Black);
        // Display what has been drawn so far
        window.display();
    }
    return 0;
}
