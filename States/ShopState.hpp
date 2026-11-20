#pragma once

#include "State.hpp"
#include "../Header/CharacterConfig.hpp"
#include "../assets/Button.hpp"
#include "../library/Stats/Stats_point.hpp"
#include "../library/Systems/Economy/inventory/myBag.hpp"

struct MerchantOffer {
    enum class Type {
        Potion,
        Consumable,
        Weapon,
        Armor
    };

    Type type;
    int id;
    std::string name;
    std::string details;
    int price;
    stats bonuses{};
};

class ShopState : public State
{
public:
    ShopState(sf::RenderWindow* window,
              std::map<std::string, int>* supportedKeys,
              std::stack<State*>* states,
              const CharacterConfig& character,
              Inventory& inventory,
              bool& equipmentChanged,
              int& coins);
    ~ShopState() override;

    void update(const float& dt) override;
    void updateInput(const float& dt) override;
    void render(sf::RenderTarget* target = NULL) override;

private:
    const CharacterConfig& character;
    Inventory& inventory;
    bool& equipmentChanged;
    int& coins;
    sf::Font font;
    sf::Font boldFont;
    sf::RectangleShape background;
    sf::RectangleShape panel;
    sf::Text titleText;
    sf::Text coinsText;
    sf::Text statusText;
    std::vector<MerchantOffer> offers;
    std::vector<sf::RectangleShape> offerCards;
    std::vector<sf::Text> offerTexts;
    std::map<std::string, Button*> buttons;
    bool actionButtonLatched = false;

    void initKeybinds();
    void generateOffers();
    void initInterface();
    void updateButtons();
    void updateCoinsText();
    void purchase(std::size_t offerIndex);
};
