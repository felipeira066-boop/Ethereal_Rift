#pragma once

#include "State.hpp"
#include "../Resource_Files/Button.hpp"

class DefeatState : public State
{
public:
    DefeatState(sf::RenderWindow* window,
                std::map<std::string, int>* supportedKeys,
                std::stack<State*>* states);
    ~DefeatState() override;

    void update(const float& dt) override;
    void updateInput(const float& dt) override;
    void render(sf::RenderTarget* target = NULL) override;

private:
    sf::Font font;
    sf::RectangleShape background;
    sf::RectangleShape panel;
    sf::Text titleText;
    std::map<std::string, Button*> buttons;
    bool actionButtonLatched = false;

    void initKeybinds() override;
    void updateButtons();
    void restartFromProfile();
};
