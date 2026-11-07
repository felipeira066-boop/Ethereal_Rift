#pragma once

#include "State.hpp"
#include "../Resource_Files/Button.hpp"
#include "../library/Systems/Economy/inventory/myBag.hpp"
#include "../library/Systems/Itens/EquipmentCatalog.hpp"

#include <functional>
#include <utility>
#include <vector>

class InventoryState : public State
{
public:
    InventoryState(sf::RenderWindow* window,
                   std::map<std::string, int>* supportedKeys,
                   std::stack<State*>* states,
                   Inventory& inventory,
                   const std::vector<EquipmentOption>& weapons,
                   const std::vector<EquipmentOption>& armors,
                   int& equippedWeapon,
                   int& equippedArmor,
                   const std::function<void()>& onEquipmentChanged,
                   const std::function<void(bool, int)>& onItemUsed);
    ~InventoryState() override;

    void update(const float& dt) override;
    void updateInput(const float& dt) override;
    void render(sf::RenderTarget* target = NULL) override;

private:
    sf::Font font;
    sf::Font boldFont;
    sf::RectangleShape background;
    sf::RectangleShape panel;
    sf::Text titleText;
    sf::Text itemsText;
    sf::Text equipmentText;
    sf::Text equipmentChartTitle;
    sf::Text previousEquipmentLabel;
    sf::Text currentEquipmentLabel;
    std::vector<sf::ConvexShape> equipmentChartGrid;
    sf::ConvexShape previousEquipmentChart;
    sf::ConvexShape currentEquipmentChart;
    std::vector<sf::Text> equipmentChartLabels;
    sf::RectangleShape modalBackdrop;
    sf::RectangleShape modalPanel;
    sf::Text modalTitle;
    std::map<std::string, Button*> buttons;
    Inventory& inventory;
    const std::vector<EquipmentOption>& weapons;
    const std::vector<EquipmentOption>& armors;
    int& equippedWeapon;
    int& equippedArmor;
    std::function<void()> onEquipmentChanged;
    std::function<void(bool, int)> onItemUsed;
    std::vector<sf::Text> itemTexts;
    std::vector<std::pair<bool, int>> itemIDs;
    bool itemModalOpen = false;
    bool selectedItemIsPotion = false;
    int selectedItemID = 0;
    std::string selectedItemName;
    stats previousEquipmentStats{};
    bool comparingWeapon = true;
    bool actionButtonLatched = false;
    bool closeKeyLatched = false;

    void initKeybinds();
    void refreshItems();
    void updateButtons();
    void updateEquipmentText();
    void initEquipmentChart();
    void updateEquipmentChart();
};
