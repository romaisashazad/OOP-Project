# Duck Shooting Game

A duck shooting game built in C++ using SFML. The game starts simple—with only slow yellow ducks—but becomes progressively harder as your score increases. More duck types begin appearing, movement speeds rise, and missing or hitting the wrong ducks costs you hearts. The goal is to survive as long as possible and end with the highest score you can.

# Gameplay Overview
1. Move the shooter using the arrow keys
2. Shoot yellow ducks → +10 points
3. Once you reach 30 points, green and brown ducks start appearing
4. Hitting a non-yellow duck → –15 points
5. Hitting two non-yellow ducks in a row → lose one heart
6. Missing a yellow duck (letting it fly off the right side of the screen) → lose one heart
7. You begin with 3 hearts. When hearts reach 0, the game ends.
8. Duck speed increases as your score rises.
9. The game starts with background sound/music.
10.  Press ESC to return to menu and ENTER to start/restart

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
g++ Duck.o Shooter.o Background.o GameState.o Game.o main.o -o duck_game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
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

##Development Notes

This project was completed collaboratively but with some version-control challenges.
Romaisa had issues committing through Git (which is why the repository shows fewer commits than expected), so much of the work was exchanged through ZIP files over WhatsApp.

Division of work:

Romaisa: Implemented the state transitions, and the sprite-based gameplay inside the GameState.cpp and GameState.hpp and Game files.

Aqsa: Handled the UI screens, background rendering, and music integration (Background.cpp/.hpp, Shooter.cpp/.hpp, and Duck.cpp/.hpp).

Although the UML originally included a timer class, we chose not to implement it and stop the game when no lives remain.
Music is also integrated directly inside the GameState system rather than in a separate audio manager.
