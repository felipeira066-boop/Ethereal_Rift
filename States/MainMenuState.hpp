#pragma once

#include "setPerfilState.h"


class MainMenuState : public State
{
    private:
        //variable
        sf::Texture bgTexture;
        sf::RectangleShape background;
        sf::Font font;

        std::map<std::string, Button*> buttons;

        //Functions
        void initVariables();
        void initBackground();
        void initFonts();
        void initKeybinds();
        void initButton();
    public:
        MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
        virtual ~MainMenuState();

        //functions 

        void update(const float& dt);
        void updateButtons();
        void renderButtons(sf::RenderTarget* target = NULL);
        void updateInput(const float& dt);
        void render(sf::RenderTarget* target = NULL);
};