#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Duck.hpp"
#include "Shooter.hpp"

//POLYMORPHISM and ABSTRACT CLASSES
class GameState{ //abstract class, therefore cannot instantiate a GameState object directly 
protected:
    sf::RenderWindow& window;
    sf::Font& font;
    bool fontLoaded;
    sf::Music* music;
    
public:
    GameState(sf::RenderWindow& win, sf::Font& f, bool loaded, sf::Music* bgm);
    virtual ~GameState();
    virtual void handleInput(sf::Event& event) = 0;
    virtual void update() = 0;
    virtual void render() = 0; //pure virtual function , must be implemented in every derived class 
    virtual GameState* getNextState() = 0;
};

// Welcome screen
class WelcomeState : public GameState{ //inherits all protected members of the parent class, therefore no need to declare its own copies of these members
private:
    GameState* nextState;
    
public:
    WelcomeState(sf::RenderWindow& win, sf::Font& f, bool loaded, sf::Music* bgm);
    void handleInput(sf::Event& event) override;
    void update() override;
    void render() override;
    GameState* getNextState() override;
};

//Instructions screen
class InstructionsState : public GameState
{
private:
    GameState* nextState; 
    
public:
    InstructionsState(sf::RenderWindow& win, sf::Font& f, bool loaded, sf::Music* bgm);
    void handleInput(sf::Event& event) override;
    void update() override;
    void render() override;
    GameState* getNextState() override;
};

// Playing state
class PlayingState : public GameState
{
private:
    Shooter shooter;
    std::vector<Duck> ducks;
    sf::Clock spawnClock;
    int& score;
    int& missed;
    bool isGameOver;
    
    // Three duck textures for different types
    sf::Texture yellowDuckTexture;
    sf::Texture greenDuckTexture;
    sf::Texture brownDuckTexture;
    bool texturesLoaded;
    
    sf::Texture heartTexture;
    bool heartTextureLoaded;
    int consecutiveNonYellowHits = 0; 
    void removeDead();
    DuckType chooseDuckType(int currentScore);  // Decides which duck type to spawn
    sf::Texture& getTextureForType(DuckType type);  // Returns appropriate texture
    
public:
    PlayingState(sf::RenderWindow& win, sf::Font& f, bool loaded, int& scoreRef, int& missedRef, sf::Music* bgm);
    void handleInput(sf::Event& event) override;
    void update() override;
    void render() override;
    GameState* getNextState() override;
    bool checkGameOver() const;
};

// Game over state
class GameOverState : public GameState
{
private:
    int finalScore;
    
public:
    GameOverState(sf::RenderWindow& win, sf::Font& f, bool loaded, int score, sf::Music* bgm);
    void handleInput(sf::Event& event) override;
    void update() override;
    void render() override;
    GameState* getNextState() override;
};

#endif