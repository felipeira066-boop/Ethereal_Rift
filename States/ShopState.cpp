#include "ShopState.hpp"

#include "../Header/Palette.hpp"
#include "../library/Systems/Itens/EquipmentCatalog.hpp"

#include <algorithm>
#include <array>
#include <random>
#include <sstream>
#include <stdexcept>

namespace {
void appendEquipmentOffers(std::vector<MerchantOffer>& offers,
                           const std::vector<EquipmentOption>& equipment,
                           const std::array<int, 3>& prices,
                           MerchantOffer::Type type,
                           const Inventory& inventory)
{
    for (std::size_t index = 1; index < equipment.size(); ++index) {
        const EquipmentOption& item = equipment[index];
        if (type == MerchantOffer::Type::Weapon
                ? inventory.hasWeapon(item.id)
                : inventory.hasArmor(item.id)) {
            continue;
        }
        std::ostringstream details;
        details << "VIT +" << item.bonuses.VIG
                << "  DEF +" << item.bonuses.DEF
                << "  SPD +" << item.bonuses.VEL << "\n"
                << "STR +" << item.bonuses.FOR
                << "  INT +" << item.bonuses.INT;
        offers.push_back({
            type, item.id, item.name, details.str(),
            prices[index - 1], item.bonuses
        });
    }
}

std::string effectDescription(const effectsBonus& bonus)
{
    std::ostringstream details;
    bool hasPrevious = false;
    const auto appendEffect = [&details, &hasPrevious](const std::string& name,
                                                       int value) {
        if (value == 0) {
            return;
        }
        if (hasPrevious) {
            details << "  ";
        }
        details << name << (value > 0 ? " +" : " ") << value;
        hasPrevious = true;
    };

    appendEffect("HP", bonus.HealAmount);
    appendEffect("Mana", bonus.ManaBonus);
    appendEffect("Damage", bonus.DamageBonus);
    appendEffect("Shield", bonus.ShieldBonus);
    appendEffect("Speed", bonus.SpeedBonus);
    appendEffect("Critical", bonus.CrtBonus);
    if (bonus.LeftTurns > 0) {
        details << "  (" << bonus.LeftTurns << " turns)";
    }
    return details.str();
}

}

ShopState::ShopState(sf::RenderWindow* window,
                     std::map<std::string, int>* supportedKeys,
                     std::stack<State*>* states,
                     const CharacterConfig& character,
                     Inventory& inventory,
                     bool& equipmentChanged,
                     int& coins)
    : State(window, supportedKeys, states),
      character(character),
      inventory(inventory),
      equipmentChanged(equipmentChanged),
      coins(coins),
      titleText(this->boldFont, "Arena Merchant", 30),
      coinsText(this->font, "", 20),
      statusText(this->font, "", 16)
{
    if (!this->font.openFromFile("Fonts/slkscr.ttf")) {
        throw "ERROR::SHOP_STATE::INIT_FONT";
    }
    if (!this->boldFont.openFromFile("Fonts/slkscrb.ttf")) {
        throw "ERROR::SHOP_STATE::INIT_BOLD_FONT";
    }

    this->initKeybinds();
    this->generateOffers();
    this->initInterface();
}

ShopState::~ShopState()
{
    for (auto& button : this->buttons) {
        delete button.second;
    }
}

void ShopState::initKeybinds()
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

void ShopState::generateOffers()
{
    std::vector<MerchantOffer> supplies;
    const std::array<int, 9> potionPrices = {
        10, 25, 50, 10, 25, 60, 15, 35, 70
    };
    for (int id = 1; id <= 9; ++id) {
        const auto potionId = static_cast<PotionID>(id);
        const PotionItem* potion = this->inventory.getPotion(potionId);
        if (potion) {
            supplies.push_back({
                MerchantOffer::Type::Potion, id, potion->name,
                effectDescription(potion->Bonus),
                potionPrices[static_cast<std::size_t>(id - 1)], {}
            });
        }
    }

    const std::array<int, 4> consumablePrices = {15, 25, 35, 50};
    for (int id = 1; id <= 4; ++id) {
        const auto consumableId = static_cast<ConsumableID>(id);
        const ConsumableItem* consumable =
            this->inventory.getEffect(consumableId);
        if (consumable) {
            supplies.push_back({
                MerchantOffer::Type::Consumable, id, consumable->name,
                effectDescription(consumable->Bonus),
                consumablePrices[static_cast<std::size_t>(id - 1)], {}
            });
        }
    }

    const std::vector<EquipmentOption> weapons =
        getWeaponsForClass(this->character.characterClass);
    const std::vector<EquipmentOption> armors =
        getArmorForClass(this->character.characterClass);
    std::array<int, 3> weaponPrices{};
    std::array<int, 3> armorPrices{};
    if (this->character.characterClass == "Hunter") {
        weaponPrices = {40, 60, 90};
        armorPrices = {35, 75, 55};
    } else if (this->character.characterClass == "Mage") {
        weaponPrices = {45, 80, 130};
        armorPrices = {65, 40, 100};
    } else if (this->character.characterClass == "Cleric") {
        weaponPrices = {70, 90, 150};
        armorPrices = {25, 50, 110};
    } else {
        weaponPrices = {50, 75, 120};
        armorPrices = {30, 60, 130};
    }

    std::vector<MerchantOffer> equipment;
    appendEquipmentOffers(equipment, weapons, weaponPrices,
                          MerchantOffer::Type::Weapon, this->inventory);
    appendEquipmentOffers(equipment, armors, armorPrices,
                          MerchantOffer::Type::Armor, this->inventory);

    std::random_device randomDevice;
    std::mt19937 randomEngine(randomDevice());
    std::shuffle(supplies.begin(), supplies.end(), randomEngine);
    std::shuffle(equipment.begin(), equipment.end(), randomEngine);
    if (supplies.size() < 3) {
        throw std::runtime_error("Shop catalog does not have enough supplies.");
    }

    this->offers.insert(this->offers.end(), supplies.begin(),
                        supplies.begin() + 3);
    const std::size_t equipmentOfferCount =
        std::min<std::size_t>(6, equipment.size());
    this->offers.insert(this->offers.end(), equipment.begin(),
                        equipment.begin() + equipmentOfferCount);
}

void ShopState::initInterface()
{
    this->background.setSize(sf::Vector2f(1280.f, 720.f));
    this->background.setFillColor(theme::darkOliveGreen);

    this->panel.setPosition(sf::Vector2f(24.f, 20.f));
    this->panel.setSize(sf::Vector2f(1232.f, 680.f));
    this->panel.setFillColor(theme::laurelGreen);
    this->panel.setOutlineColor(theme::camel);
    this->panel.setOutlineThickness(2.f);

    this->titleText.setPosition(sf::Vector2f(48.f, 30.f));
    this->titleText.setFillColor(theme::russet);
    this->coinsText.setPosition(sf::Vector2f(48.f, 77.f));
    this->coinsText.setFillColor(theme::russet);
    this->statusText.setPosition(sf::Vector2f(610.f, 80.f));
    this->statusText.setFillColor(theme::russet);
    this->updateCoinsText();

    for (std::size_t index = 0; index < this->offers.size(); ++index) {
        const std::size_t column = index % 3;
        const std::size_t row = index / 3;
        const float x = 34.f + static_cast<float>(column) * 408.f;
        const float y = 116.f + static_cast<float>(row) * 172.f;

        sf::RectangleShape card;
        card.setPosition(sf::Vector2f(x, y));
        card.setSize(sf::Vector2f(390.f, 158.f));
        card.setFillColor(theme::darkOliveGreen);
        card.setOutlineColor(theme::camel);
        card.setOutlineThickness(2.f);
        this->offerCards.push_back(card);

        sf::Text text(this->font, "", 16);
        text.setPosition(sf::Vector2f(x + 14.f, y + 12.f));
        text.setFillColor(theme::cornsilk);
        const MerchantOffer& offer = this->offers[index];
        const std::string category =
            offer.type == MerchantOffer::Type::Potion ||
                    offer.type == MerchantOffer::Type::Consumable
                ? "Consumable"
                : offer.type == MerchantOffer::Type::Weapon
                    ? "Weapon"
                    : "Armor";
        text.setString(category + ": " + offer.name + "\n" +
                       offer.details + "\nPrice: " +
                       std::to_string(offer.price) + " coins");
        this->offerTexts.push_back(text);

        const sf::Color idle = theme::russet;
        const sf::Color hover = theme::alloyOrange;
        const sf::Color active = theme::camel;
        this->buttons["BUY_" + std::to_string(index)] = new Button(
            x + 70.f, y + 105.f, 250.f, 42.f, &this->font, "Buy",
            idle, hover, active);
    }

    this->buttons["CLOSE"] = new Button(
        505.f, 648.f, 270.f, 46.f, &this->font, "Continue",
        theme::russet, theme::alloyOrange, theme::camel);
}

void ShopState::updateInput(const float&)
{
    const auto closeKey = this->keybinds.find("CLOSE");
    if (closeKey != this->keybinds.end() &&
        sf::Keyboard::isKeyPressed(
            static_cast<sf::Keyboard::Scancode>(closeKey->second))) {
        this->endState();
    }
}

void ShopState::update(const float& dt)
{
    this->updateMousePosition();
    this->updateInput(dt);
    this->updateButtons();
}

void ShopState::updateButtons()
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

    for (std::size_t index = 0; index < this->offers.size(); ++index) {
        if (this->buttons["BUY_" + std::to_string(index)]->isPressed()) {
            this->actionButtonLatched = true;
            this->purchase(index);
            return;
        }
    }

    if (this->buttons["CLOSE"]->isPressed()) {
        this->actionButtonLatched = true;
        this->endState();
    }
}

void ShopState::updateCoinsText()
{
    this->coinsText.setString("Coins: " + std::to_string(this->coins));
}

void ShopState::purchase(std::size_t offerIndex)
{
    if (offerIndex >= this->offers.size()) {
        this->statusText.setString("Invalid offer.");
        return;
    }

    const MerchantOffer& offer = this->offers[offerIndex];
    if (this->coins < offer.price) {
        this->statusText.setString("Not enough coins.");
        return;
    }

    bool addedToInventory = true;
    if (offer.type == MerchantOffer::Type::Potion) {
        addedToInventory =
            this->inventory.addPotion(static_cast<PotionID>(offer.id));
    } else if (offer.type == MerchantOffer::Type::Consumable) {
        addedToInventory =
            this->inventory.addConsumable(static_cast<ConsumableID>(offer.id));
    } else if (offer.type == MerchantOffer::Type::Weapon) {
        if (this->inventory.hasWeapon(offer.id)) {
            this->statusText.setString("Already owned.");
            return;
        }
        addedToInventory = this->inventory.addWeapon(offer.id);
        this->equipmentChanged = addedToInventory;
    } else {
        if (this->inventory.hasArmor(offer.id)) {
            this->statusText.setString("Already owned.");
            return;
        }
        addedToInventory = this->inventory.addArmor(offer.id);
        this->equipmentChanged = addedToInventory;
    }

    if (!addedToInventory) {
        this->statusText.setString("Could not store the item.");
        return;
    }

    this->coins -= offer.price;
    this->updateCoinsText();
    this->statusText.setString("Purchased: " + offer.name);
}

void ShopState::render(sf::RenderTarget* target)
{
    if (!target) {
        target = this->window;
    }

    target->draw(this->background);
    target->draw(this->panel);
    target->draw(this->titleText);
    target->draw(this->coinsText);
    target->draw(this->statusText);
    for (std::size_t index = 0; index < this->offerCards.size(); ++index) {
        target->draw(this->offerCards[index]);
        target->draw(this->offerTexts[index]);
    }
    for (auto& button : this->buttons) {
        button.second->render(target);
    }
}
