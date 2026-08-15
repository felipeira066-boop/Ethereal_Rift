#pragma once

#include "../States/MainMenuState.hpp"


class Game{
    private:
        //Variable
        sf::RenderWindow *window;
        sf::Event event;
        std::vector<sf::VideoMode> videoModes;
        sf::ContextSettings windowSettings;
        bool fullscreen;

        sf::Clock dtClock;
        float dt;

        std::stack<State*> states;
        std::map<std::string, int> supportedKeys;

        //Init
        void initVariables();
        void initWindow();
        void initKeys();
        void initState();

    public:
        //COnstructions & Destructions
        Game();
        virtual ~Game();

        //Functions
        void updateDt();
        void updateSFMLEvents();
        void update();
        void render();
        void run();
};