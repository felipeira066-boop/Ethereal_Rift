#ifndef SETPERFILSTATE_H
#define SETPERFILSTATE_H

#pragma once

#include "BattleState.hpp"
#include "../assets/Button.hpp"
#include <array>
#include <cmath>
#include "../Header/CharacterConfig.hpp"

class setPerfilState : public State
{
public:
    setPerfilState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
    virtual ~setPerfilState();

    void update(const float& dt);   
    void handleEvent(const sf::Event& event);
    void updateButton();
    void renderButton(sf::RenderTarget* target = NULL);
    void updateInput(const float& dt);
    void render(sf::RenderTarget* target = NULL);

private:
    /*Variables*/
    sf::RectangleShape background;
    sf::Font font;
    sf::Font boldFont;
    sf::Text selectedRaceText;
    sf::Text selectedClassText;
    sf::Text nameText;
    sf::RectangleShape nameInputBox;
    sf::Text instructionsText;
    std::string characterName;
    std::string selectedRace;
    std::string selectedClass;
    std::array<float, 5> selectedStats;
    sf::ConvexShape radarGrid;
    sf::ConvexShape radarValues;
    std::vector<sf::Text> radarLabels;

    std::map<std::string, Button*> buttons;

    void initVariables();
    void initBackground();
    void initFonts();
    void initKeybinds();
    void initButton();
    void initRadarChart();
    void updateRadarChart();
    void updateSelectedStats();
    sf::Vector2f radarPoint(float value, float radius) const;
    CharacterConfig buildCharacterConfig() const;

};

#endif