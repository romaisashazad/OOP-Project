#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Background.hpp"
#include "GameState.hpp"

class Game
{
private:
    const unsigned int WIDTH;
    const unsigned int HEIGHT;
    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded;
    Background background;

    sf::Music bgMusic;    //  (music is now a member variable)

    GameState* currentState;
    int score;
    int missed;

    static const int STATE_WELCOME = 0;
    static const int STATE_INSTRUCTIONS = 1;
    static const int STATE_PLAYING = 2;
    static const int STATE_GAME_OVER = 3;
    int stateType;

    void changeState(int newStateType);
    void handleEvents();
    void handleStateTransitions(sf::Event& event);
    void update();
    void render();

public:
    Game();
    ~Game();
    void run();
};

#endif
