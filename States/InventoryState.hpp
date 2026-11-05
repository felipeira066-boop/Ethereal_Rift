#pragma once

#include "State.hpp"
#include "../Resource_Files/Button.hpp"
#include "../library/Systems/Economy/inventory/myBag.hpp"
#include "../library/Systems/Itens/EquipmentCatalog.hpp"

#include <functional>

class InventoryState : public State
{
public:
    InventoryState(sf::RenderWindow* window,
                   std::map<std::string, int>* supportedKeys,
                   std::stack<State*>* states,
                   const Inventory& inventory,
                   const std::vector<EquipmentOption>& weapons,
                   const std::vector<EquipmentOption>& armors,
                   int& equippedWeapon,
                   int& equippedArmor,
                   const std::function<void()>& onEquipmentChanged);
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
    sf::Text equipmentText;
    std::map<std::string, Button*> buttons;
    const std::vector<EquipmentOption>& weapons;
    const std::vector<EquipmentOption>& armors;
    int& equippedWeapon;
    int& equippedArmor;
    std::function<void()> onEquipmentChanged;
    bool actionButtonLatched = false;

    void initKeybinds();
    void updateButtons();
    void updateEquipmentText();
};
