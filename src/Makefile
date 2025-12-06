# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall

SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
# Target executable
TARGET = duck_game

# Source files
SOURCES = main.cpp Game.cpp GameState.cpp Duck.cpp Shooter.cpp Background.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files
HEADERS = Game.hpp GameState.hpp Duck.hpp Shooter.hpp Background.hpp

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(SFML_FLAGS)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run