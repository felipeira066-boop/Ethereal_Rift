CXX := g++

CXXFLAGS := -Wall -pipe -I.

SFMLFLAGS := -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lsfml-network


TARGET := builds/Ethereal_Rift.exe

SRC_DIR := src 

SRCS = src/main.cpp src/Game.cpp \
		States/State.cpp States/GameState.cpp States/MainMenuState.cpp \
		Resource_Files/Button.cpp\

.PHONY: all clean

all: $(TARGET)

$(SRCS): $(shell find library -name "*.hpp")

$(TARGET) : $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET)  $(SFMLFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
