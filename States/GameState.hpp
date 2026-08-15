#pragma once

#include "State.hpp"

class GameState : public State
{
    private:

        //Functions
        void initKeybinds();
        void initTextures();
        void initPlayer();
    public:
        GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
        virtual ~GameState();

        //functions 
        
        void update(const float& dt);
        void updateInput(const float& dt);
        void render(sf::RenderTarget* target = NULL);
};