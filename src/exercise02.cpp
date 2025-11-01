#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 300), "STOP Sign");
    sf::Color backgroundColor=sf::Color::Cyan;

    // Create a circle shape with radius 80
    // Set the position of the circle in the center of the window
    // circle.setPosition(400 - 50, 300 - 50); // (window width/2 - radius, window height/2 - radius)

    sf::CircleShape circle(80);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineThickness(20);
    circle.setOutlineColor(sf::Color::Red);
    circle.setPosition(200-80, 150-80); //set circle to the centre of the screen

    sf::RectangleShape post(sf::Vector2f(30, 200));//vertical rect touching the bottom of the cirlce and situated right the centre 
    post.setFillColor(sf::Color::Black);
    float coordinates_x = 200;
    float red_part = 80 + 20;
    float coordinates_y = 150 + red_part;
    post.setPosition(coordinates_x - 15, coordinates_y);

    sf::Font Font; //font 
    if(!Font.loadFromFile("Arial.ttf")){
        return -1;
    }
    sf::Text Text;
    Text.setFont(Font);
    Text.setString("STOP");
    Text.setCharacterSize(50); //font size
    Text.setFillColor(sf::Color::Black);
    sf::FloatRect Text_pos=Text.getLocalBounds();
    Text.setOrigin(Text_pos.left + Text_pos.width/2, Text_pos.top + Text_pos.height/2);
    Text.setPosition(200, 150);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(backgroundColor);
        window.draw(circle); // Draw the circle on the window
        window.draw(post);
        window.draw(Text);
        window.display(); // Display the contents
    }

    return 0;
}