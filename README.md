# Duck Shooting Game

A simple duck shooting game built with SFML and C++.

## File Structure

```
Duck.hpp / Duck.cpp          - Duck entity class
Shooter.hpp / Shooter.cpp    - Shooter/crosshair class
Background.hpp / Background.cpp - Background rendering
GameState.hpp / GameState.cpp   - All game state classes (Welcome, Instructions, Playing, GameOver)
Game.hpp / Game.cpp          - Main game manager
main.cpp                     - Entry point
Makefile                     - Build configuration
```

## Requirements

- C++ compiler with C++11 support (g++)
- SFML 2.x library installed
- Arial.ttf font file in the same directory

## Compilation

### Using Makefile:
```bash
make
```

### Manual compilation:
```bash
g++ -std=c++11 -c Duck.cpp
g++ -std=c++11 -c Shooter.cpp
g++ -std=c++11 -c Background.cpp
g++ -std=c++11 -c GameState.cpp
g++ -std=c++11 -c Game.cpp
g++ -std=c++11 -c main.cpp
g++ Duck.o Shooter.o Background.o GameState.o Game.o main.o -o duck_game -lsfml-graphics -lsfml-window -lsfml-system
```

## Running

```bash
./duck_game
```

Or with make:
```bash
make run
```

## Clean Build

```bash
make clean
```

## How to Play

- Use ARROW KEYS to move the cyan shooter
- Touch the yellow ducks to shoot them
- +10 points per duck
- Let 3 ducks escape and you lose
- Press ESC to return to menu
- Press ENTER to start/restart