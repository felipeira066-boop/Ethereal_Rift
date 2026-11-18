#pragma once

#include "State.hpp"
#include "../Header/CharacterConfig.hpp"
#include "../Resource_Files/Button.hpp"
#include "../library/Arena.hpp"
#include "../library/Stats/HP.hpp"
#include "../library/Stats/Mana.hpp"
#include "../library/character/Moves/Moves.hpp"
#include "../library/Stats/level.hpp"
#include "../library/Systems/Economy/inventory/myBag.hpp"
#include "../library/Systems/Itens/EquipmentCatalog.hpp"

class BattleState : public State
{
public:
    BattleState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys,
                std::stack<State*>* states, const CharacterConfig& character);
    ~BattleState() override;

    void update(const float& dt) override;
    void updateInput(const float& dt) override;
    void render(sf::RenderTarget* target = NULL) override;
    void startEncounter(std::size_t enemyIndex, std::size_t worldMobIndex);
    bool isWorldMobDefeated(std::size_t worldMobIndex) const;
    sf::Vector2f getWorldPlayerPosition() const;
    void setWorldPlayerPosition(const sf::Vector2f& position);
    void returnToWorld();
    void openShop();

private:
    CharacterConfig character;
    sf::Font font;
    sf::Font boldFont;
    sf::RectangleShape background;
    sf::RectangleShape hudPanel;
    sf::RectangleShape arenaPanel;
    sf::RectangleShape playerHpTrack;
    sf::RectangleShape playerHpBar;
    sf::RectangleShape manaTrack;
    sf::RectangleShape manaBar;
    sf::RectangleShape expTrack;
    sf::RectangleShape expBar;
    sf::RectangleShape enemyHpTrack;
    sf::RectangleShape enemyHpBar;
    sf::RectangleShape playerShape;
    sf::RectangleShape enemyShape;
    sf::Text playerNameText;
    sf::Text playerStatsText;
    sf::Text playerHpText;
    sf::Text manaText;
    sf::Text expText;
    sf::Text levelText;
    sf::Text enemyNameText;
    sf::Text enemyHpText;
    sf::Text messageText;
    std::map<std::string, Button*> buttons;

    stats playerBaseStats{};
    stats equippedWeaponStats{};
    stats equippedArmorStats{};
    finalStats playerFinalStats{};
    HP playerHealth;
    Mana playerMana;
    Inventory inventory;
    std::vector<EquipmentOption> weapons;
    std::vector<EquipmentOption> armors;
    int equippedWeapon = 0;
    int equippedArmor = 0;
    int playerCriticalRate = 0;
    Exp progression;
    std::vector<EnemyData> enemies;
    std::size_t enemyIndex = 0;
    std::size_t currentWorldMobIndex = 0;
    std::vector<bool> defeatedWorldMobs = std::vector<bool>(6, false);
    sf::Vector2f worldPlayerPosition = {120.f, 350.f};
    EnemyData currentEnemy{};
    finalStats enemyFinalStats{};
    HP enemyHealth;
    const std::vector<MoveData>* moveTable = nullptr;
    bool gameFinished = false;
    bool shopEquipmentChanged = false;
    bool actionButtonLatched = false;
    float playerHitFlashRemaining = 0.f;
    float playerHitFlashElapsed = 0.f;
    float enemyHitFlashRemaining = 0.f;
    float enemyHitFlashElapsed = 0.f;

    void initKeybinds();
    void initInterface();
    void selectMoveTable();
    stats levelGrowth() const;
    void recalculatePlayerStats();
    void equipSelectedItems();
    void refreshOwnedEquipment();
    void syncEquipmentSelections();
    void loadEnemy();
    void updateBars();
    void updateHitFlashes(float dt);
    void updateButtons();
    void flashWhenHit(sf::RectangleShape& shape, float& remaining,
                      float& elapsed);
    void showDefeatState();
    void useMove(std::size_t moveIndex);
    void playEnemyTurn();
    void finishEnemy();
    void drawBar(sf::RenderTarget* target, const sf::RectangleShape& track,
                const sf::RectangleShape& bar, const sf::Text& label);
};
