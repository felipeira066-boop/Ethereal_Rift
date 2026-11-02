#pragma once

#include "../States/MainMenuState.hpp"


class Game{
    private:
        //Variable
        sf::RenderWindow *window;
        std::vector<sf::VideoMode> videoModes;
        sf::ContextSettings windowSettings;
        sf::View view;
        bool fullscreen;

        sf::Clock dtClock;
        float dt;

        std::stack<State*> states;
        std::map<std::string, int> supportedKeys;

        //Init
        void initVariables();
        void initWindow();
        void updateView(const sf::Vector2u& windowSize);
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