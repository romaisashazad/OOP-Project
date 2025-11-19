
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 300), "Instructions Screen");

    sf::Color backgroundColor(135, 206, 235);

    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        return -1;
    }

    sf::Text title;
    title.setFont(font);
    title.setString("HOW TO PLAY");
    title.setCharacterSize(22);
    title.setFillColor(sf::Color::White);
    title.setOutlineColor(sf::Color(44, 62, 80));
    title.setOutlineThickness(2.f);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition((500 - titleBounds.width) / 2, 15);

    sf::Text instructions;
    instructions.setFont(font);
    instructions.setCharacterSize(14);
    instructions.setFillColor(sf::Color::Black);
    instructions.setLineSpacing(1.2f);
    instructions.setPosition(30, 60);
    instructions.setString(
        "1. Shoot ducks before 3 escape\n\n"
        "2. Move cursor to aim and shoot\n\n"
        "3. +10 points for each duck hit\n\n"
        "4. Miss 3 ducks and game ends\n\n"
        "5. Good luck shooting some ducks :)"
    );

    sf::RectangleShape startButton(sf::Vector2f(110, 30));
    startButton.setFillColor(sf::Color::Green);
    startButton.setPosition(280, 250);

    sf::Text startText;
    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(14);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(299, 255);

    sf::RectangleShape exitButton(sf::Vector2f(80, 30));
    exitButton.setFillColor(sf::Color(128, 0, 0));
    exitButton.setPosition(400, 250);

    sf::Text exitText;
    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(14);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(425, 255);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));

                if (startButton.getGlobalBounds().contains(mouse)) {
                    window.close(); // change to game start later
                }

                if (exitButton.getGlobalBounds().contains(mouse)) {
                    window.close();
                }
            }
        }

        // Get mouse position
        sf::Vector2f mousePos(sf::Mouse::getPosition(window));

        // Hover effect for Start Button
        if (startButton.getGlobalBounds().contains(mousePos)) {
            startButton.setFillColor(sf::Color(0, 200, 0)); // lighter green
        } else {
            startButton.setFillColor(sf::Color::Green);
        }

        // Hover effect for Exit Button
        if (exitButton.getGlobalBounds().contains(mousePos)) {
            exitButton.setFillColor(sf::Color(200, 0, 0)); // lighter red
        } else {
            exitButton.setFillColor(sf::Color(128, 0, 0));
        }

        window.clear(backgroundColor);
        window.draw(title);
        window.draw(instructions);
        window.draw(startButton);
        window.draw(startText);
        window.draw(exitButton);
        window.draw(exitText);
        window.display();
    }

    return 0;
}

