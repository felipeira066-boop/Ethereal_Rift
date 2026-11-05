#include "InventoryState.hpp"
#include "Header/Palette.hpp"

#include <sstream>

InventoryState::InventoryState(sf::RenderWindow* window,
                               std::map<std::string, int>* supportedKeys,
                               std::stack<State*>* states,
                               const Inventory& inventory,
                               const std::vector<EquipmentOption>& weapons,
                               const std::vector<EquipmentOption>& armors,
                               int& equippedWeapon,
                               int& equippedArmor,
                               const std::function<void()>& onEquipmentChanged)
    : State(window, supportedKeys, states),
      titleText(this->font, "Inventario", 36),
      itemsText(this->font, "", 21),
      equipmentText(this->font, "", 20),
      weapons(weapons),
      armors(armors),
      equippedWeapon(equippedWeapon),
      equippedArmor(equippedArmor),
      onEquipmentChanged(onEquipmentChanged)
{
    if (!this->font.openFromFile("Fonts/GeistPixel-Regular-VariableFont_ELSH.ttf")) {
        throw "ERROR::INVENTORY_STATE::INIT_FONT";
    }

    this->initKeybinds();
    this->background.setSize(sf::Vector2f(1280.f, 720.f));
    this->background.setFillColor(theme::darkOliveGreen);

    this->panel.setPosition(sf::Vector2f(160.f, 80.f));
    this->panel.setSize(sf::Vector2f(960.f, 540.f));
    this->panel.setFillColor(theme::laurelGreen);
    this->panel.setOutlineColor(theme::camel);
    this->panel.setOutlineThickness(2.f);

    this->titleText.setPosition(sf::Vector2f(210.f, 110.f));
    this->titleText.setFillColor(theme::russet);

    std::ostringstream items;
    items << "Itens: " << inventory.getTotalItems() << "/"
          << inventory.getCapacity() << "\n\n";
    bool hasItems = false;

    for (int index = 1; index <= 4; ++index) {
        const auto id = static_cast<ConsumableID>(index);
        const int quantity = inventory.getConsumableQuantity(id);
        if (quantity > 0) {
            items << inventory.getConsumableName(id) << "  x" << quantity << "\n";
            hasItems = true;
        }
    }

    for (int index = 1; index <= 9; ++index) {
        const auto id = static_cast<PotionID>(index);
        const int quantity = inventory.getPotionQuantity(id);
        if (quantity > 0) {
            items << inventory.getPotionName(id) << "  x" << quantity << "\n";
            hasItems = true;
        }
    }

    if (!hasItems) {
        items << "Seu inventario esta vazio.";
    }

    this->itemsText.setString(items.str());
    this->itemsText.setPosition(sf::Vector2f(220.f, 190.f));
    this->itemsText.setFillColor(theme::russet);

    this->equipmentText.setPosition(sf::Vector2f(755.f, 190.f));
    this->equipmentText.setFillColor(theme::russet);
    this->updateEquipmentText();

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
        515.f, 535.f, 250.f, 56.f, &this->font, "Voltar a batalha",
        theme::russet, theme::alloyOrange, theme::camel);
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
    if (closeKey != this->keybinds.end() &&
        sf::Keyboard::isKeyPressed(
            static_cast<sf::Keyboard::Scancode>(closeKey->second))) {
        this->endState();
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

    const auto changeEquipment = [this](const std::string& buttonName,
                                        int& equipped,
                                        const std::vector<EquipmentOption>& options,
                                        int direction) {
        if (!this->buttons[buttonName]->isPressed()) {
            return false;
        }
        const int count = static_cast<int>(options.size());
        equipped = (equipped + direction + count) % count;
        this->onEquipmentChanged();
        this->updateEquipmentText();
        this->actionButtonLatched = true;
        return true;
    };

    if (changeEquipment("WEAPON_PREVIOUS", this->equippedWeapon,
                       this->weapons, -1) ||
        changeEquipment("WEAPON_NEXT", this->equippedWeapon,
                        this->weapons, 1) ||
        changeEquipment("ARMOR_PREVIOUS", this->equippedArmor,
                        this->armors, -1) ||
        changeEquipment("ARMOR_NEXT", this->equippedArmor,
                        this->armors, 1)) {
        return;
    }

    if (this->buttons["BACK"]->isPressed()) {
        this->actionButtonLatched = true;
        this->endState();
    }
}

void InventoryState::updateEquipmentText()
{
    std::ostringstream equipment;
    equipment << "EQUIPAMENTO\n\n"
              << "Arma:\n" << this->weapons.at(this->equippedWeapon).name
              << "\n\n\nArmadura:\n"
              << this->armors.at(this->equippedArmor).name;
    this->equipmentText.setString(equipment.str());
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
    target->draw(this->equipmentText);
    for (auto& button : this->buttons) {
        button.second->render(target);
    }
}
