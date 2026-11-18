#include "InventoryState.hpp"
#include "Header/Palette.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <sstream>

namespace {
const EquipmentOption* getEquipmentOption(
    const std::vector<EquipmentOption>& options, int index)
{
    if (index < 0 || static_cast<std::size_t>(index) >= options.size()) {
        return nullptr;
    }
    return &options[static_cast<std::size_t>(index)];
}

std::array<float, 6> getChartStats(const stats& values)
{
    return {
        static_cast<float>(values.VIG),
        static_cast<float>(values.DEF),
        static_cast<float>(values.VEL),
        static_cast<float>(values.FOR),
        static_cast<float>(values.INT),
        static_cast<float>(values.FOR + values.INT)
    };
}
}

InventoryState::InventoryState(sf::RenderWindow* window,
                               std::map<std::string, int>* supportedKeys,
                               std::stack<State*>* states,
                               Inventory& inventory,
                               const std::vector<EquipmentOption>& weapons,
                               const std::vector<EquipmentOption>& armors,
                               int& equippedWeapon,
                               int& equippedArmor,
                               const std::function<void()>& onEquipmentChanged,
                               const std::function<void(bool, int)>& onItemUsed)
    : State(window, supportedKeys, states),
      titleText(this->boldFont, "Inventory", 36),
      itemsText(this->font, "", 21),
      equipmentText(this->font, "", 18),
      equipmentChartTitle(this->boldFont, "", 18),
      previousEquipmentLabel(this->font, "Previous", 14),
      currentEquipmentLabel(this->font, "Equipped", 14),
      modalTitle(this->font, "", 22),
      inventory(inventory),
      weapons(weapons),
      armors(armors),
      equippedWeapon(equippedWeapon),
      equippedArmor(equippedArmor),
      onEquipmentChanged(onEquipmentChanged),
      onItemUsed(onItemUsed)
{
    if (!this->font.openFromFile("Fonts/slkscr.ttf")) {
        throw "ERROR::INVENTORY_STATE::INIT_FONT";
    }
    if (!this->boldFont.openFromFile("Fonts/slkscrb.ttf")) {
        throw "ERROR::INVENTORY_STATE::INIT_BOLD_FONT";
    }

    this->initKeybinds();
    if (!getEquipmentOption(this->weapons, this->equippedWeapon)) {
        this->equippedWeapon = this->weapons.empty() ? -1 : 0;
    }
    if (!getEquipmentOption(this->armors, this->equippedArmor)) {
        this->equippedArmor = this->armors.empty() ? -1 : 0;
    }
    this->background.setSize(sf::Vector2f(1280.f, 720.f));
    this->background.setFillColor(theme::darkOliveGreen);

    this->panel.setPosition(sf::Vector2f(160.f, 80.f));
    this->panel.setSize(sf::Vector2f(960.f, 540.f));
    this->panel.setFillColor(theme::laurelGreen);
    this->panel.setOutlineColor(theme::camel);
    this->panel.setOutlineThickness(2.f);

    this->titleText.setPosition(sf::Vector2f(210.f, 110.f));
    this->titleText.setFillColor(theme::russet);

    this->itemsText.setPosition(sf::Vector2f(220.f, 190.f));
    this->itemsText.setLineSpacing(1.15f);
    this->itemsText.setFillColor(theme::russet);
    this->refreshItems();

    this->equipmentText.setPosition(sf::Vector2f(755.f, 190.f));
    this->equipmentText.setLineSpacing(1.15f);
    this->equipmentText.setFillColor(theme::russet);
    this->updateEquipmentText();
    this->initEquipmentChart();

    this->buttons["WEAPON_PREVIOUS"] = new Button(
        680.f, 260.f, 64.f, 48.f, &this->font, "<",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["WEAPON_NEXT"] = new Button(
        1040.f, 260.f, 64.f, 48.f, &this->font, ">",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["ARMOR_PREVIOUS"] = new Button(
        680.f, 390.f, 64.f, 48.f, &this->font, "<",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["ARMOR_NEXT"] = new Button(
        1040.f, 390.f, 64.f, 48.f, &this->font, ">",
        theme::russet, theme::alloyOrange, theme::camel);

    this->buttons["BACK"] = new Button(
        515.f, 560.f, 250.f, 48.f, &this->font, "Back to Battle",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["USE_ITEM"] = new Button(
        475.f, 375.f, 145.f, 52.f, &this->font, "Use",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["CANCEL_ITEM"] = new Button(
        660.f, 375.f, 145.f, 52.f, &this->font, "Back",
        theme::russet, theme::alloyOrange, theme::camel);

    this->modalBackdrop.setSize(sf::Vector2f(1280.f, 720.f));
    this->modalBackdrop.setFillColor(sf::Color(0, 0, 0, 150));
    this->modalPanel.setPosition(sf::Vector2f(420.f, 245.f));
    this->modalPanel.setSize(sf::Vector2f(440.f, 220.f));
    this->modalPanel.setFillColor(theme::laurelGreen);
    this->modalPanel.setOutlineColor(theme::camel);
    this->modalPanel.setOutlineThickness(3.f);
    this->modalTitle.setPosition(sf::Vector2f(450.f, 285.f));
    this->modalTitle.setFillColor(theme::russet);
}

InventoryState::~InventoryState()
{
    for (auto& button : this->buttons) {
        delete button.second;
    }
}

void InventoryState::initKeybinds()
{
    std::ifstream file("Config/GameStates_keybinds.ini");
    std::string action;
    std::string key;

    while (file >> action >> key) {
        const auto supportedKey = this->supportedKeys->find(key);
        if (supportedKey != this->supportedKeys->end()) {
            this->keybinds[action] = supportedKey->second;
        }
    }
}

void InventoryState::updateInput(const float&)
{
    const auto closeKey = this->keybinds.find("CLOSE");
    if (closeKey == this->keybinds.end()) {
        return;
    }

    const bool closePressed = sf::Keyboard::isKeyPressed(
        static_cast<sf::Keyboard::Scancode>(closeKey->second));
    if (!closePressed) {
        this->closeKeyLatched = false;
    } else if (!this->closeKeyLatched) {
        this->closeKeyLatched = true;
        if (this->itemModalOpen) {
            this->itemModalOpen = false;
        } else {
            this->endState();
        }
    }
}

void InventoryState::update(const float& dt)
{
    this->updateMousePosition();
    this->updateInput(dt);
    this->updateButtons();
}

void InventoryState::updateButtons()
{
    for (auto& button : this->buttons) {
        button.second->update(this->mousePosView);
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        this->actionButtonLatched = false;
    }
    if (this->actionButtonLatched) {
        return;
    }

    if (this->itemModalOpen) {
        if (this->buttons["CANCEL_ITEM"]->isPressed()) {
            this->itemModalOpen = false;
            this->actionButtonLatched = true;
        } else if (this->buttons["USE_ITEM"]->isPressed()) {
            const bool consumed = this->selectedItemIsPotion
                ? this->inventory.consumePotion(
                    static_cast<PotionID>(this->selectedItemID))
                : this->inventory.consumeConsumable(
                    static_cast<ConsumableID>(this->selectedItemID));
            if (consumed) {
                this->onItemUsed(
                    this->selectedItemIsPotion, this->selectedItemID);
                this->refreshItems();
                this->itemModalOpen = false;
            }
            this->actionButtonLatched = true;
        }
        return;
    }

    const auto changeEquipment = [this](const std::string& buttonName,
                                        int& equipped,
                                        const std::vector<EquipmentOption>& options,
                                         int direction,
                                         bool isWeapon) {
        if (!this->buttons[buttonName]->isPressed()) {
            return false;
        }

        if (options.empty()) {
            this->actionButtonLatched = true;
            return true;
        }
        if (equipped < 0 ||
            static_cast<std::size_t>(equipped) >= options.size()) {
            equipped = 0;
        }

        const int count = static_cast<int>(options.size());
        const stats previous = options.at(
            static_cast<std::size_t>(equipped)).bonuses;
        equipped = (equipped + direction + count) % count;
        this->previousEquipmentStats = previous;
        this->comparingWeapon = isWeapon;
        this->onEquipmentChanged();
        this->updateEquipmentText();
        this->updateEquipmentChart();
        this->actionButtonLatched = true;
        return true;
    };

    if (changeEquipment("WEAPON_PREVIOUS", this->equippedWeapon,
                        this->weapons, -1, true) ||
        changeEquipment("WEAPON_NEXT", this->equippedWeapon,
                         this->weapons, 1, true) ||
        changeEquipment("ARMOR_PREVIOUS", this->equippedArmor,
                        this->armors, -1, false) ||
        changeEquipment("ARMOR_NEXT", this->equippedArmor,
                         this->armors, 1, false)) {
        return;
    }

    if (this->buttons["BACK"]->isPressed()) {
        this->actionButtonLatched = true;
        this->endState();
        return;
    }

    for (std::size_t index = 0; index < this->itemTexts.size(); ++index) {
        if (this->itemTexts[index].getGlobalBounds().contains(
                this->mousePosView)) {
            this->selectedItemIsPotion = this->itemIDs[index].first;
            this->selectedItemID = this->itemIDs[index].second;
            this->selectedItemName = this->selectedItemIsPotion
                ? this->inventory.getPotionName(
                    static_cast<PotionID>(this->selectedItemID))
                : this->inventory.getConsumableName(
                    static_cast<ConsumableID>(this->selectedItemID));
            this->modalTitle.setString(
                "Use " + this->selectedItemName + "?");
            this->itemModalOpen = true;
            this->actionButtonLatched = true;
            return;
        }
    }
}

void InventoryState::refreshItems()
{
    this->itemTexts.clear();
    this->itemIDs.clear();

    std::ostringstream header;
    header << "Items: " << this->inventory.getTotalItems() << "/"
           << this->inventory.getCapacity() << "\n\n"
           << "Click an item to use it:";
    this->itemsText.setString(header.str());

    float rowY = 252.f;
    const auto addItem = [this, &rowY](bool isPotion, int id,
                                       const std::string& name, int quantity) {
        if (quantity <= 0) {
            return;
        }

        this->itemTexts.emplace_back(
            this->font, name + "  x" + std::to_string(quantity), 20);
        sf::Text& text = this->itemTexts.back();
        text.setPosition(sf::Vector2f(220.f, rowY));
        text.setFillColor(theme::russet);
        this->itemIDs.emplace_back(isPotion, id);
        rowY += 26.f;
    };

    for (int index = 1; index <= 4; ++index) {
        const auto id = static_cast<ConsumableID>(index);
        addItem(false, index, this->inventory.getConsumableName(id),
                this->inventory.getConsumableQuantity(id));
    }
    for (int index = 1; index <= 9; ++index) {
        const auto id = static_cast<PotionID>(index);
        addItem(true, index, this->inventory.getPotionName(id),
                this->inventory.getPotionQuantity(id));
    }

    if (this->itemTexts.empty()) {
        this->itemsText.setString(
            "Items: 0/" + std::to_string(this->inventory.getCapacity()) +
            "\n\nYour inventory is empty.");
    }
}

void InventoryState::updateEquipmentText()
{
    const EquipmentOption* weapon =
        getEquipmentOption(this->weapons, this->equippedWeapon);
    const EquipmentOption* armor =
        getEquipmentOption(this->armors, this->equippedArmor);
    std::ostringstream equipment;
    equipment << "EQUIPMENT\n\n"
              << "Weapon:\n" << (weapon ? weapon->name : "None")
              << "\n\nArmor:\n"
              << (armor ? armor->name : "None");
    this->equipmentText.setString(equipment.str());
}

void InventoryState::initEquipmentChart()
{
    constexpr std::size_t axisCount = 6;
    constexpr float chartX = 900.f;
    constexpr float chartY = 485.f;
    constexpr float chartRadius = 70.f;
    constexpr float pi = 3.14159265358979323846f;

    this->equipmentChartTitle.setPosition(sf::Vector2f(775.f, 338.f));
    this->equipmentChartTitle.setFillColor(theme::russet);
    this->previousEquipmentLabel.setPosition(sf::Vector2f(775.f, 365.f));
    this->previousEquipmentLabel.setFillColor(theme::russet);
    this->currentEquipmentLabel.setPosition(sf::Vector2f(900.f, 365.f));
    this->currentEquipmentLabel.setFillColor(theme::alloyOrange);

    this->equipmentChartGrid.clear();
    for (int ring = 1; ring <= 4; ++ring) {
        sf::ConvexShape shape;
        shape.setPointCount(axisCount);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color(
            theme::lemonMeringue.r, theme::lemonMeringue.g,
            theme::lemonMeringue.b, 150));
        shape.setOutlineThickness(1.f);
        const float radius = chartRadius * static_cast<float>(ring) / 4.f;
        for (std::size_t axis = 0; axis < axisCount; ++axis) {
            const float angle = -pi / 2.f +
                2.f * pi * static_cast<float>(axis) /
                    static_cast<float>(axisCount);
            shape.setPoint(axis, sf::Vector2f(
                chartX + std::cos(angle) * radius,
                chartY + std::sin(angle) * radius));
        }
        this->equipmentChartGrid.push_back(shape);
    }

    this->previousEquipmentChart.setPointCount(axisCount);
    this->previousEquipmentChart.setFillColor(sf::Color(
        theme::russet.r, theme::russet.g, theme::russet.b, 90));
    this->previousEquipmentChart.setOutlineColor(theme::russet);
    this->previousEquipmentChart.setOutlineThickness(2.f);
    this->currentEquipmentChart.setPointCount(axisCount);
    this->currentEquipmentChart.setFillColor(sf::Color(
        theme::alloyOrange.r, theme::alloyOrange.g,
        theme::alloyOrange.b, 100));
    this->currentEquipmentChart.setOutlineColor(theme::alloyOrange);
    this->currentEquipmentChart.setOutlineThickness(2.f);

    const std::array<std::string, axisCount> names = {
        "VIT", "DEF", "SPD", "STR", "INT", "DMG"
    };
    for (std::size_t axis = 0; axis < axisCount; ++axis) {
        this->equipmentChartLabels.emplace_back(this->font, names[axis], 14);
        sf::Text& label = this->equipmentChartLabels.back();
        label.setFillColor(theme::cornsilk);
        const float angle = -pi / 2.f +
            2.f * pi * static_cast<float>(axis) /
                static_cast<float>(axisCount);
        const sf::Vector2f point(
            chartX + std::cos(angle) * (chartRadius + 14.f),
            chartY + std::sin(angle) * (chartRadius + 14.f));
        const sf::FloatRect bounds = label.getLocalBounds();
        label.setPosition(sf::Vector2f(
            point.x - bounds.size.x / 2.f,
            point.y - bounds.size.y / 2.f));
    }

    this->updateEquipmentChart();
}

void InventoryState::updateEquipmentChart()
{
    constexpr std::size_t axisCount = 6;
    constexpr float chartX = 900.f;
    constexpr float chartY = 485.f;
    constexpr float chartRadius = 70.f;
    constexpr float pi = 3.14159265358979323846f;

    const EquipmentOption* equippedOption = this->comparingWeapon
        ? getEquipmentOption(this->weapons, this->equippedWeapon)
        : getEquipmentOption(this->armors, this->equippedArmor);
    const stats equipped = equippedOption ? equippedOption->bonuses : stats{};
    const std::array<float, axisCount> previousValues =
        getChartStats(this->previousEquipmentStats);
    const std::array<float, axisCount> currentValues = getChartStats(equipped);
    const float maximum = std::max(
        1.f, *std::max_element(currentValues.begin(), currentValues.end()));
    const float scale = std::max(
        maximum,
        *std::max_element(previousValues.begin(), previousValues.end()));

    this->equipmentChartTitle.setString(
        this->comparingWeapon ? "Weapon comparison" : "Armor comparison");
    for (std::size_t axis = 0; axis < axisCount; ++axis) {
        const float angle = -pi / 2.f +
            2.f * pi * static_cast<float>(axis) /
                static_cast<float>(axisCount);
        const float previousRadius =
            chartRadius * previousValues[axis] / scale;
        const float currentRadius =
            chartRadius * currentValues[axis] / scale;
        this->previousEquipmentChart.setPoint(axis, sf::Vector2f(
            chartX + std::cos(angle) * previousRadius,
            chartY + std::sin(angle) * previousRadius));
        this->currentEquipmentChart.setPoint(axis, sf::Vector2f(
            chartX + std::cos(angle) * currentRadius,
            chartY + std::sin(angle) * currentRadius));
    }
}

void InventoryState::render(sf::RenderTarget* target)
{
    if (!target) {
        target = this->window;
    }

    target->draw(this->background);
    target->draw(this->panel);
    target->draw(this->titleText);
    target->draw(this->itemsText);
    for (sf::Text& itemText : this->itemTexts) {
        if (!this->itemModalOpen &&
            itemText.getGlobalBounds().contains(this->mousePosView)) {
            itemText.setFillColor(sf::Color(209, 96, 77));
        } else {
            itemText.setFillColor(theme::russet);
        }
        target->draw(itemText);
    }
    target->draw(this->equipmentText);
    target->draw(this->equipmentChartTitle);
    target->draw(this->previousEquipmentLabel);
    target->draw(this->currentEquipmentLabel);
    for (sf::ConvexShape& ring : this->equipmentChartGrid) {
        target->draw(ring);
    }
    target->draw(this->previousEquipmentChart);
    target->draw(this->currentEquipmentChart);
    for (sf::Text& label : this->equipmentChartLabels) {
        target->draw(label);
    }
    for (auto& button : this->buttons) {
        if (button.first != "USE_ITEM" && button.first != "CANCEL_ITEM") {
            button.second->render(target);
        }
    }

    if (this->itemModalOpen) {
        target->draw(this->modalBackdrop);
        target->draw(this->modalPanel);
        target->draw(this->modalTitle);
        this->buttons["USE_ITEM"]->render(target);
        this->buttons["CANCEL_ITEM"]->render(target);
    }
}
