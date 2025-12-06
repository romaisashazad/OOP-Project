#include "GameState.hpp"
#include <string>
#include <cstdlib>
#include <iostream>
#include <algorithm>
using namespace std;

//////////////////////////////BASE CLASS GAMESTATE//////////////////////////

GameState::GameState(sf::RenderWindow& win, sf::Font& f, bool loaded, sf::Music* bgm): window(win), font(f), fontLoaded(loaded), music(bgm){}

GameState::~GameState(){}

////////////////////////////////WELCOME STATE////////////////////////////////

WelcomeState::WelcomeState(sf::RenderWindow& win, sf::Font& f, bool loaded, sf::Music* bgm): GameState(win, f, loaded, bgm), nextState(nullptr){}

void WelcomeState::handleInput(sf::Event& event){
    if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::Escape){
            window.close();
        }
    }
}
void WelcomeState::update(){}

void WelcomeState::render(){
    if(!fontLoaded){
        return;
    }

    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    sf::Text title("DUCK SHOOTING GAME", font, 50);
    title.setFillColor(sf::Color::Yellow);
    title.setStyle(sf::Text::Bold);

    sf::FloatRect bounds = title.getLocalBounds();
    title.setOrigin(bounds.left + bounds.width/2, bounds.top + bounds.height/2);

    title.setPosition(windowWidth/2, windowHeight * 0.4f); 
    window.draw(title);

    sf::Text inst("Press I for Instructions", font, 26); 
    inst.setFillColor(sf::Color::Black);
    inst.setStyle(sf::Text::Bold);


    //object of type floatrect, that stores dimenions 
    sf::FloatRect ib = inst.getLocalBounds(); inst.setOrigin(ib.left + ib.width/2, ib.top + ib.height / 2);

    //above grass ≈ 85% of screen height
    inst.setPosition(windowWidth/2, windowHeight * 0.85f);

    window.draw(inst);
}
GameState* WelcomeState::getNextState(){ //stores the address of a GameState object, else nullptr if no next state selected therefore no transition 
    return nextState;
}

////////////////////////////////INSTRUCTIONS STATE////////////////////////////////

InstructionsState::InstructionsState(sf::RenderWindow& win, sf::Font& f, bool loaded, sf::Music* bgm): GameState(win, f, loaded, bgm), nextState(nullptr){}

void InstructionsState::handleInput(sf::Event& event){}

void InstructionsState::update(){}

void InstructionsState::render(){
    if (!fontLoaded)
        return;

    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(26);
    text.setFillColor(sf::Color::White);

    std::string instructions =
        "                  HOW TO PLAY:\n\n"
        "1. Use ARROW KEYS to move the shooter\n"
        "2. Touch the yellow ducks to shoot them!\n"
        "3. You get +10 points per yellow duck\n"
        "5. Let 3 ducks escape and you lose.\n\n"
        "                Press ENTER to start";

    text.setString(instructions);

    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin(bounds.left + bounds.width/2, bounds.top);

    float marginX = 40.f;
    float textWidth = bounds.width;
    float scaleFactor = 1.0f;

    if (textWidth > windowWidth - 2 * marginX)
    {
        scaleFactor = (windowWidth - 2 * marginX)/textWidth;
        text.setCharacterSize(static_cast<unsigned int>(28 * scaleFactor));
        bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width/2, bounds.top);
    }

    text.setPosition(windowWidth/2, windowHeight * 0.24f);
    window.draw(text);
}

GameState* InstructionsState::getNextState(){
    return nextState;
}

////////////////////////////////PLAYING STATE////////////////////////////////
PlayingState::PlayingState(sf::RenderWindow& win, sf::Font& f, bool loaded, int& scoreRef, int& missedRef, sf::Music* bgm): GameState(win, f, loaded, bgm),
      shooter(),
      score(scoreRef),
      missed(missedRef),
      isGameOver(false),
      texturesLoaded(false),
      heartTextureLoaded(false),
      consecutiveNonYellowHits(0)
{
    score = 0;
    missed = 0;

    bool yellowLoaded = false, greenLoaded = false, brownLoaded = false;

    if(yellowDuckTexture.loadFromFile("yellowduck_nobg.png")) yellowLoaded = true;
    if(greenDuckTexture.loadFromFile("greenduck_nobg.png")) greenLoaded = true;
    if(brownDuckTexture.loadFromFile("brownduck_nobg.png")) brownLoaded = true;

    texturesLoaded = yellowLoaded && greenLoaded && brownLoaded;

    if(heartTexture.loadFromFile("hearty_nobg.png")) heartTextureLoaded = true;

    if(music){
        music->setVolume(40);
        music->setLoop(true);
        music->play(); //starts the background music
    }
}

void PlayingState::update(){
    float speed = 4.5f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  shooter.move(-speed, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) shooter.move(speed, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    shooter.move(0, -speed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  shooter.move(0, speed);

    float spawnTime = std::max(1.5f, 2.8f - (score/100.0f) * 0.6f);

    if(texturesLoaded && spawnClock.getElapsedTime().asSeconds() > spawnTime){
        float duckSpeed = 2.0f + (score / 20) * 0.3f;
        int ducksToSpawn = (score >= 100) ? 3 : (score >= 60) ? 2 : 1;

        for(int i = 0; i < ducksToSpawn; i++){
            float y = 150 + rand() % 300;
            DuckType type = chooseDuckType(score);
            sf::Texture& texture = getTextureForType(type);
            float xOffset = -40.f - (i * 80.f);
            ducks.emplace_back(xOffset, y, texture, 0.15f, duckSpeed, type);
        }

        spawnClock.restart();
    }

    for(Duck& duck : ducks){
        duck.update();

        if(duck.isHit(shooter.getPosition())){
            if(duck.getType() == YELLOW){
                score += 10;
                consecutiveNonYellowHits = 0; //reset counter
            }
            else{
                score -= 15;
                consecutiveNonYellowHits++;

                if (consecutiveNonYellowHits >= 2)
                {
                    missed++;  //lose one heart
                    consecutiveNonYellowHits = 0;
                }
            }
        }

        if(duck.isOffScreen() && duck.isAlive()){
            if (duck.getType() == YELLOW) missed++;
        }
    }

    removeDead();

    if(missed >= 3 || score < 0){
        isGameOver = true;
    }
}

void PlayingState::render(){
    // Score (left-top)
    if (fontLoaded){
        sf::Text scoreText("Score: " + std::to_string(score), font, 20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 8);
        window.draw(scoreText);
    }
    // Hearts (top-right)
    if(heartTextureLoaded){
        int lives = 3 - missed;

        float heartScale = 0.08f;
        float heartSize = heartTexture.getSize().x * heartScale;
        float heartSpacing = 8.f;

        float windowWidth = window.getSize().x;
        float startX = windowWidth - (lives * heartSize + (lives - 1) * heartSpacing) - 10.f;
        float yPos = 8.f;

        for (int i = 0; i < lives; i++)
        {
            sf::Sprite heartSprite;
            heartSprite.setTexture(heartTexture);
            heartSprite.setScale(heartScale, heartScale);

            float xPos = startX + i * (heartSize + heartSpacing);
            heartSprite.setPosition(xPos, yPos);

            window.draw(heartSprite);
        }
    }

    // Ducks
    for (Duck& duck : ducks) duck.draw(window);

    // Shooter
    shooter.draw(window);
}

GameState* PlayingState::getNextState(){
    return nullptr;
}

bool PlayingState::checkGameOver() const{
    return isGameOver;
}

void PlayingState::removeDead(){
    std::vector<Duck> alive;
    for (Duck& duck : ducks)
    {
        if (duck.isAlive() && !duck.isOffScreen())
        {
            alive.push_back(duck);
        }
    }
    ducks = alive;
}
void PlayingState::handleInput(sf::Event& event){}

// Decides which duck type to spawn based on current score
DuckType PlayingState::chooseDuckType(int currentScore)
{
    // Before score 20, only yellow ducks
    if (currentScore < 20)
    {
        return YELLOW;
    }
    
    // After score 30: Introduce green and brown ducks with higher frequency
    int random = rand() % 100;
    
    if (currentScore < 60){
        if (random < 50)
            return YELLOW;
        else if (random < 75)
            return GREEN;
        else
            return BROWN;
    }
    else if (currentScore < 100){
        if (random < 40)
            return YELLOW;
        else if (random < 70)
            return GREEN;
        else
            return BROWN;
    }
    else{
        if (random < 30)
            return YELLOW;
        else if (random < 65)
            return GREEN;
        else
            return BROWN;
    }
}

sf::Texture& PlayingState::getTextureForType(DuckType type){
    switch(type){
        case YELLOW:
            return yellowDuckTexture;
        case GREEN:
            return greenDuckTexture;
        case BROWN:
            return brownDuckTexture;
        default:
            return yellowDuckTexture;
    }
}

////////////////////////////////GameOverState/////////////////////////////
GameOverState::GameOverState(sf::RenderWindow& win, sf::Font& f, bool loaded, int score, sf::Music* bgm): GameState(win, f, loaded, bgm), finalScore(score){
    if(music){
        music->stop(); //stops the music once the game is no longer in playing state
    }
} //copy int score into finalScore data member

void GameOverState::handleInput(sf::Event& event){} //could have included exit logic here

void GameOverState::update(){}

void GameOverState::render(){
    if (!fontLoaded)
        return;

    float winWidth = static_cast<float>(window.getSize().x); //window width
    float winHeight = static_cast<float>(window.getSize().y); //h

    sf::Text go("GAME OVER", font, 44);
    go.setFillColor(sf::Color::Red);
    go.setStyle(sf::Text::Bold);
    go.setOrigin(go.getLocalBounds().width/2, go.getLocalBounds().height/2);
    go.setPosition(winWidth/2, winHeight * 0.2f);
    window.draw(go);

    sf::Text final("FINAL SCORE: " + std::to_string(finalScore), font, 28);
    final.setFillColor(sf::Color::Yellow);
    final.setOrigin(final.getLocalBounds().width/2, final.getLocalBounds().height/2);
    final.setPosition(winWidth/2, winHeight * 0.2f + 60);
    window.draw(final);

    sf::Text retry("Press ENTER to try again", font, 20);
    retry.setFillColor(sf::Color::White);
    retry.setOrigin(retry.getLocalBounds().width/2, retry.getLocalBounds().height/2);
    retry.setPosition(winWidth/2, winHeight * 0.85f);
    window.draw(retry);
}

GameState* GameOverState::getNextState(){
    return nullptr;
}