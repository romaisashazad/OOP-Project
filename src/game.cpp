#include "Game.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>

Game::Game(): WIDTH(800), HEIGHT(600), window(sf::VideoMode(WIDTH, HEIGHT), "Duck Shooting Game",
         sf::Style::Close),
  background(WIDTH, HEIGHT),
  currentState(nullptr),
  score(0), missed(0),
  stateType(STATE_WELCOME){
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    srand(time(nullptr));

    fontLoaded = font.loadFromFile("Arial.ttf");
    if(!fontLoaded)
        return;
    if(!bgMusic.openFromFile("ducksound.mp3")){
        return;
    }else{
        bgMusic.setLoop(true);
        bgMusic.setVolume(50);
        bgMusic.play();
    }

    changeState(STATE_WELCOME);
}

Game::~Game(){
    delete currentState;
}

void Game::changeState(int newStateType){
    delete currentState;
    stateType = newStateType;

    if (stateType == STATE_WELCOME)
        currentState = new WelcomeState(window, font, fontLoaded, &bgMusic);

    else if (stateType == STATE_INSTRUCTIONS)
        currentState = new InstructionsState(window, font, fontLoaded, &bgMusic);

    else if (stateType == STATE_PLAYING)
        currentState = new PlayingState(window, font, fontLoaded, score, missed, &bgMusic);

    else if (stateType == STATE_GAME_OVER)
        currentState = new GameOverState(window, font, fontLoaded, score, &bgMusic);
}

void Game::handleEvents(){
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed)
            handleStateTransitions(event);

        currentState->handleInput(event);
    }
}

void Game::handleStateTransitions(sf::Event& event){
    if(stateType == STATE_WELCOME){
        if(event.key.code == sf::Keyboard::Enter){
            changeState(STATE_PLAYING);
        }
        if(event.key.code == sf::Keyboard::I){
            changeState(STATE_INSTRUCTIONS);
        }
    }else if(stateType == STATE_INSTRUCTIONS){
        if(event.key.code == sf::Keyboard::Enter){
            changeState(STATE_PLAYING);
        }
        if(event.key.code == sf::Keyboard::Escape){
            changeState(STATE_WELCOME);
        }
    }else if(stateType == STATE_PLAYING){
        if(event.key.code == sf::Keyboard::Escape){
            changeState(STATE_WELCOME);
        }
    }else if(stateType == STATE_GAME_OVER){
        if(event.key.code == sf::Keyboard::Enter){
            changeState(STATE_WELCOME);
        }
    }
}

void Game::update(){
    currentState->update();

    if(stateType == STATE_PLAYING){
        auto* p = dynamic_cast<PlayingState*>(currentState);
        if(p && p->checkGameOver()){
            changeState(STATE_GAME_OVER);
        }
    }
}

void Game::render(){
    background.render(window);
    currentState->render();
    window.display();
}

void Game::run(){
    while(window.isOpen()){
        handleEvents();
        update();
        render();
    }
}
