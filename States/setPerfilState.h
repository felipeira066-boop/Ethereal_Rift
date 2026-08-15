#ifndef SETPERFILSTATE_H
#define SETPERFILSTATE_H

#pragma once

#include "GameState.hpp"
#include "../Resource_Files/Button.hpp"

class setPerfilState : public State
{
public:
    setPerfilState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
    virtual ~setPerfilState();

    void update(const float& dt);   
    void updateButton();
    void renderButton(sf::RenderTarget* target = NULL);
    void updateInput(const float& dt);
    void render(sf::RenderTarget* target = NULL);

private:
    /*Variables*/
    sf::Texture bgTexture;
    sf::RectangleShape background;
    sf::Font font;

    std::map<std::string, Button*> buttons;

    void initVariables();
    void initBackground();
    void initFonts();
    void initKeybinds();
    void initButton();

};

#endif