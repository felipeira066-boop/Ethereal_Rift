#pragma once

#include "State.hpp"
#include "../Header/CharacterConfig.hpp"
#include "../Resource_Files/Button.hpp"

class GameState : public State
{
    private:

        //Functions
        void initKeybinds();
        void initTextures();
        void initPlayer();
    public:
        GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys,
                  std::stack<State*>* states, const CharacterConfig& character);
        virtual ~GameState();

        //functions 
        
        void update(const float& dt);
        void updateInput(const float& dt);
        void render(sf::RenderTarget* target = NULL);

    private:
        CharacterConfig character;
        sf::Font font;
        sf::Font boldFont;
        sf::Text characterText;
        sf::Text statsText;
        sf::Text arenaText;
        sf::Text monsterText;
        sf::Text hitboxText;
        sf::RectangleShape hudPanel;
        sf::RectangleShape arenaPanel;
        sf::RectangleShape monsterHitbox;
        sf::RectangleShape playerHitbox;
        std::map<std::string, Button*> buttons;

        void initInterface();
        void updateButtons();
        void renderInterface(sf::RenderTarget* target);
};