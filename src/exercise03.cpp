#include <SFML/Graphics.hpp>

int main() {
    // Create a window of size 400x300
    sf::RenderWindow window(sf::VideoMode(400, 300), "Bouncing Ball");

    // Create a ball (circle) with radius 50
    sf::CircleShape ball(20);
    ball.setFillColor(sf::Color::White);
    
    // Initial position of the ball (starting near the center)
    ball.setPosition(180, 1);

    // Ball velocity (initially moving in both X and Y directions)
    sf::Vector2f velocity(0.04f, 0.04f); // X and Y velocities

    //paddle
    sf::RectangleShape paddle(sf::Vector2f(100, 10));
    paddle.setFillColor(sf::Color::Green);
    paddle.setPosition(150, 270); //start from bottom left corner of the screen each time
    float movement_pixels = 0.2; 

    bool paused = false;
    bool is_active = true;

    sf::Font Font;
    if(!Font.loadFromFile("Arial.ttf")){
        return -1;
    }
    sf::Text Text;
    Text.setFont(Font);
    Text.setString("Press Space Bar to continue");
    Text.setCharacterSize(25);
    Text.setFillColor(sf::Color::Red);
    Text.setPosition(42, 130);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type==sf::Event::KeyPressed){ //restart game
                if(event.key.code==sf::Keyboard::Space && paused){
                    paused=false;
                    is_active=true;
                    ball.setPosition(180, 1);
                    velocity=sf::Vector2f(0.04f, 0.04f);
                    paddle.setPosition(150, 270);
                }
            }
        }
        if(!paused){
            sf::Vector2f position_of_paddle=paddle.getPosition();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && position_of_paddle.x > 0){
                paddle.move(-movement_pixels, 0);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && position_of_paddle.x + paddle.getSize().x < 400){
                paddle.move(movement_pixels, 0);
            }
            if(is_active){
                // Get the current position of the ball
                sf::Vector2f position = ball.getPosition();
                sf::Vector2f paddle_size = paddle.getSize();

                if(position.x <= 0 || position.x + ball.getRadius()*2 >= 400){
                    //check if the circumference of the ball is touching the boundaries of the screen
                    velocity.x = -velocity.x; // Reverse direction in X axis
                }
                if(position.y <= 0){
                    velocity.y = -velocity.y; // Reverse direction in Y axis
                }
                if(position.y+ball.getRadius()*2 >= position_of_paddle.y && position.y <= position_of_paddle.y + paddle_size.y && position.x + ball.getRadius()*2 >= position_of_paddle.x && position.x <= position_of_paddle.x + paddle_size.x){
                    velocity.y = -velocity.y; 
                }
                if(position.y + ball.getRadius()*2 >= 300){ //ball does not toucht the paddle therefore end game condition
                    is_active=false;
                    paused=true;
                }
                // Move the ball by the velocity
                ball.move(velocity);
            }
        }

        // Clear the screen
        window.clear();

        // // Draw the ball
        // window.draw(ball);
        window.draw(paddle);
        if(is_active){
            window.draw(ball);
        } else if(paused){
            window.draw(Text);
        }

        // Display the contents
        window.display();
    }

    return 0;
}