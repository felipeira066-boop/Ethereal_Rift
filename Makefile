CXX := g++

CXXFLAGS := -Wall -pipe -I.

SFMLFLAGS := -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lsfml-network 

TARGET := builds/Ethereal_Rift.exe 

SRC_DIR := src 

SRCS = src/main.cpp src/Game.cpp \
		States/State.cpp States/GameState.cpp States/BattleState.cpp States/InventoryState.cpp States/MainMenuState.cpp States/setPerfilState.cpp \
		States/DefeatState.cpp States/ShopState.cpp States/WorldState.cpp assets/Button.cpp

.PHONY: all run clean

all: $(TARGET)

$(TARGET) : $(SRCS)
	if not exist builds mkdir builds
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) $(SFMLFLAGS)

run: $(TARGET)
	$(TARGET)

clean:
	if exist "$(subst /,\,$(TARGET))" del /Q "$(subst /,\,$(TARGET))"
