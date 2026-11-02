#pragma once

#include "State.hpp"
#include "../Resource_Files/Button.hpp"
#include "../library/Systems/Economy/inventory/myBag.hpp"

class InventoryState : public State
{
public:
    InventoryState(sf::RenderWindow* window,
                   std::map<std::string, int>* supportedKeys,
                   std::stack<State*>* states,
                   const Inventory& inventory);
    ~InventoryState() override;

    void update(const float& dt) override;
    void updateInput(const float& dt) override;
    void render(sf::RenderTarget* target = NULL) override;

private:
    sf::Font font;
    sf::RectangleShape background;
    sf::RectangleShape panel;
    sf::Text titleText;
    sf::Text itemsText;
    std::map<std::string, Button*> buttons;
    bool actionButtonLatched = false;

    void initKeybinds();
    void updateButtons();
};
