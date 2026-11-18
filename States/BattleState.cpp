#include "BattleState.hpp"
#include "InventoryState.hpp"
#include "DefeatState.hpp"
#include "ShopState.hpp"
#include "WorldState.hpp"
#include "Header/Palette.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

extern int CurrentCoin;

namespace {
constexpr float barWidth = 430.f;
constexpr float hitFlashDuration = 0.42f;
constexpr float hitFlashInterval = 0.07f;
const sf::Color playerColor = theme::laurelGreen;
const sf::Color enemyColor = theme::russet;

void setBar(sf::RectangleShape& bar, const sf::RectangleShape& track,
            float current, float maximum)
{
    const float ratio = maximum > 0.f
        ? std::clamp(current / maximum, 0.f, 1.f)
        : 0.f;
    bar.setPosition(track.getPosition());
    bar.setSize(sf::Vector2f(track.getSize().x * ratio, track.getSize().y));
}

bool sameStats(const stats& left, const stats& right)
{
    return left.VIG == right.VIG && left.DEF == right.DEF &&
           left.VEL == right.VEL && left.FOR == right.FOR &&
           left.INT == right.INT;
}

int findEquipmentIndex(const std::vector<EquipmentOption>& options,
                       const stats& bonuses)
{
    for (std::size_t index = 0; index < options.size(); ++index) {
        if (sameStats(options[index].bonuses, bonuses)) {
            return static_cast<int>(index);
        }
    }
    return 0;
}

void replaceEquipmentStats(stats& characterStats,
                           const stats& previous,
                           const stats& replacement)
{
    characterStats.VIG += replacement.VIG - previous.VIG;
    characterStats.DEF += replacement.DEF - previous.DEF;
    characterStats.VEL += replacement.VEL - previous.VEL;
    characterStats.FOR += replacement.FOR - previous.FOR;
    characterStats.INT += replacement.INT - previous.INT;
}

std::vector<EquipmentOption> ownedEquipmentOptions(
    const std::vector<EquipmentOption>& catalog,
    const Inventory& inventory,
    bool isWeapon)
{
    std::vector<EquipmentOption> owned;
    for (const EquipmentOption& option : catalog) {
        if (option.id == 0 ||
            (isWeapon ? inventory.hasWeapon(option.id)
                      : inventory.hasArmor(option.id))) {
            owned.push_back(option);
        }
    }
    return owned;
}
}

BattleState::BattleState(sf::RenderWindow* window,
                         std::map<std::string, int>* supportedKeys,
                         std::stack<State*>* states,
                         const CharacterConfig& character)
    : State(window, supportedKeys, states),
      character(character),
      playerNameText(this->boldFont, "", 22),
      playerStatsText(this->font, "", 16),
      playerHpText(this->font, "", 16),
      manaText(this->font, "", 16),
      expText(this->font, "", 16),
      levelText(this->font, "", 18),
      enemyNameText(this->boldFont, "", 22),
      enemyHpText(this->font, "", 16),
      messageText(this->font, "", 20),
      weapons(ownedEquipmentOptions(
          getWeaponsForClass(character.characterClass), this->inventory, true)),
      armors(ownedEquipmentOptions(
          getArmorForClass(character.characterClass), this->inventory, false)),
      enemies(arenaInimigos)
{
    if (!this->font.openFromFile("Fonts/slkscr.ttf")) {
        throw "ERROR::BATTLE_STATE::INIT_FONT";
    }
    if (!this->boldFont.openFromFile("Fonts/slkscrb.ttf")) {
        throw "ERROR::BATTLE_STATE::INIT_BOLD_FONT";
    }

    this->initKeybinds();
    this->progression.initLevel();
    this->playerBaseStats = {
        static_cast<int>(std::lround(this->character.stats[0])),
        static_cast<int>(std::lround(this->character.stats[1])),
        static_cast<int>(std::lround(this->character.stats[2])),
        static_cast<int>(std::lround(this->character.stats[3])),
        static_cast<int>(std::lround(this->character.stats[4]))
    };

    this->selectMoveTable();
    this->recalculatePlayerStats();
    this->initInterface();
    this->loadEnemy();
    this->updateBars();
}

BattleState::~BattleState()
{
    for (auto& button : this->buttons) {
        delete button.second;
    }
}

void BattleState::initKeybinds()
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

void BattleState::selectMoveTable()
{
    if (this->character.characterClass == "Hunter") {
        this->moveTable = &hunterMoves;
    } else if (this->character.characterClass == "Mage") {
        this->moveTable = &mageMoves;
    } else if (this->character.characterClass == "Cleric") {
        this->moveTable = &clericMoves;
    } else {
        this->moveTable = &warriorMoves;
    }
}

stats BattleState::levelGrowth() const
{
    if (this->character.characterClass == "Hunter") {
        return {1, 1, 2, 2, 0};
    }
    if (this->character.characterClass == "Mage") {
        return {1, 0, 1, 0, 3};
    }
    if (this->character.characterClass == "Cleric") {
        return {2, 1, 0, 1, 1};
    }
    return {2, 2, 1, 2, 0};
}

void BattleState::recalculatePlayerStats()
{
    // CharacterConfig already combines race bonuses with class base stats.
    this->playerFinalStats = totalStats::contagem(
        this->playerBaseStats, raceStats{0, 0, 0, 0, 0});
    this->playerHealth.initHP(this->playerFinalStats);
    this->playerMana.initMana(this->playerFinalStats);

    std::stringstream statsText;
    statsText << "VIT " << this->playerBaseStats.VIG
              << "   DEF " << this->playerBaseStats.DEF
              << "   SPD " << this->playerBaseStats.VEL
              << "   STR " << this->playerBaseStats.FOR
              << "   INT " << this->playerBaseStats.INT;
    this->playerStatsText.setString(statsText.str());
}

void BattleState::equipSelectedItems()
{
    if (this->weapons.empty() || this->armors.empty()) {
        return;
    }
    if (this->equippedWeapon < 0 ||
        static_cast<std::size_t>(this->equippedWeapon) >= this->weapons.size()) {
        this->equippedWeapon = 0;
    }
    if (this->equippedArmor < 0 ||
        static_cast<std::size_t>(this->equippedArmor) >= this->armors.size()) {
        this->equippedArmor = 0;
    }

    const stats& newWeapon =
        this->weapons.at(static_cast<std::size_t>(this->equippedWeapon)).bonuses;
    const stats& newArmor =
        this->armors.at(static_cast<std::size_t>(this->equippedArmor)).bonuses;

    replaceEquipmentStats(this->playerBaseStats, this->equippedWeaponStats,
                          newWeapon);
    replaceEquipmentStats(this->playerBaseStats, this->equippedArmorStats,
                          newArmor);
    this->equippedWeaponStats = newWeapon;
    this->equippedArmorStats = newArmor;
    this->recalculatePlayerStats();
}

void BattleState::syncEquipmentSelections()
{
    this->equippedWeapon =
        findEquipmentIndex(this->weapons, this->equippedWeaponStats);
    this->equippedArmor =
        findEquipmentIndex(this->armors, this->equippedArmorStats);
}

void BattleState::refreshOwnedEquipment()
{
    this->weapons = ownedEquipmentOptions(
        getWeaponsForClass(this->character.characterClass),
        this->inventory, true);
    this->armors = ownedEquipmentOptions(
        getArmorForClass(this->character.characterClass),
        this->inventory, false);
    this->syncEquipmentSelections();
}

void BattleState::initInterface()
{
    this->background.setSize(sf::Vector2f(1280.f, 720.f));
    this->background.setFillColor(theme::darkOliveGreen);

    this->hudPanel.setPosition(sf::Vector2f(32.f, 24.f));
    this->hudPanel.setSize(sf::Vector2f(1216.f, 208.f));
    this->hudPanel.setFillColor(theme::darkOliveGreen);
    this->hudPanel.setOutlineColor(theme::camel);
    this->hudPanel.setOutlineThickness(2.f);

    this->arenaPanel.setPosition(sf::Vector2f(32.f, 248.f));
    this->arenaPanel.setSize(sf::Vector2f(1216.f, 340.f));
    this->arenaPanel.setFillColor(theme::laurelGreen);
    this->arenaPanel.setOutlineColor(theme::camel);
    this->arenaPanel.setOutlineThickness(2.f);

    const auto configureTrack = [](sf::RectangleShape& shape, float x, float y,
                                   sf::Color color) {
        shape.setPosition(sf::Vector2f(x, y));
        shape.setSize(sf::Vector2f(barWidth, 20.f));
        shape.setFillColor(color);
    };
    configureTrack(this->playerHpTrack, 64.f, 102.f, theme::russet);
    configureTrack(this->playerHpBar, 64.f, 102.f, theme::alloyOrange);
    configureTrack(this->manaTrack, 64.f, 140.f, theme::russet);
    configureTrack(this->manaBar, 64.f, 140.f, theme::laurelGreen);
    configureTrack(this->expTrack, 64.f, 178.f, theme::russet);
    configureTrack(this->expBar, 64.f, 178.f, theme::camel);
    configureTrack(this->enemyHpTrack, 786.f, 102.f, theme::russet);
    configureTrack(this->enemyHpBar, 786.f, 102.f, theme::camel);

    this->playerNameText.setPosition(sf::Vector2f(64.f, 54.f));
    this->playerNameText.setFillColor(theme::cornsilk);
    this->playerStatsText.setPosition(sf::Vector2f(64.f, 203.f));
    this->playerStatsText.setFillColor(theme::lemonMeringue);
    this->playerHpText.setPosition(sf::Vector2f(64.f, 80.f));
    this->playerHpText.setFillColor(theme::cornsilk);
    this->manaText.setPosition(sf::Vector2f(64.f, 118.f));
    this->manaText.setFillColor(theme::cornsilk);
    this->expText.setPosition(sf::Vector2f(64.f, 156.f));
    this->expText.setFillColor(theme::cornsilk);
    this->levelText.setPosition(sf::Vector2f(520.f, 203.f));
    this->levelText.setFillColor(theme::lemonMeringue);

    this->enemyNameText.setPosition(sf::Vector2f(786.f, 54.f));
    this->enemyNameText.setFillColor(theme::lemonMeringue);
    this->enemyHpText.setPosition(sf::Vector2f(786.f, 80.f));
    this->enemyHpText.setFillColor(theme::cornsilk);

    this->playerShape.setPosition(sf::Vector2f(230.f, 365.f));
    this->playerShape.setSize(sf::Vector2f(100.f, 125.f));
    this->playerShape.setFillColor(playerColor);
    this->playerShape.setOutlineColor(theme::cornsilk);
    this->playerShape.setOutlineThickness(3.f);
    this->enemyShape.setPosition(sf::Vector2f(940.f, 350.f));
    this->enemyShape.setSize(sf::Vector2f(135.f, 145.f));
    this->enemyShape.setFillColor(enemyColor);
    this->enemyShape.setOutlineColor(theme::alloyOrange);
    this->enemyShape.setOutlineThickness(3.f);

    this->messageText.setPosition(sf::Vector2f(64.f, 265.f));
    this->messageText.setFillColor(theme::russet);
    this->messageText.setString("Choose an ability to start your turn.");

    const float buttonY = 620.f;
    const float buttonWidth = 250.f;
    const sf::Color idle = theme::russet;
    const sf::Color hover = theme::alloyOrange;
    const sf::Color active = theme::camel;
    for (std::size_t index = 0; index < this->moveTable->size(); ++index) {
        const float buttonX = 44.f + static_cast<float>(index) * 260.f;
        this->buttons["MOVE_" + std::to_string(index)] = new Button(
            buttonX, buttonY, buttonWidth, 56.f, &this->font,
            (*this->moveTable)[index].nome, idle, hover, active);
    }
    this->buttons["FLEE"] = new Button(
        844.f, buttonY, 190.f, 56.f, &this->font, "Flee",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["INVENTORY"] = new Button(
        1050.f, buttonY, 190.f, 56.f, &this->font, "Inventory",
        theme::darkOliveGreen, theme::laurelGreen, theme::camel);
    this->playerNameText.setString(
        this->character.name + " - " + this->character.race + " "
        + this->character.characterClass);
}

void BattleState::loadEnemy()
{
    if (this->enemyIndex >= this->enemies.size()) {
        this->gameFinished = true;
        this->messageText.setString("Arena cleared! Click Flee to return.");
        return;
    }

    this->currentEnemy = this->enemies[this->enemyIndex];
    this->enemyFinalStats = {
        static_cast<std::uint32_t>(this->currentEnemy.hpmax),
        this->currentEnemy.shield,
        static_cast<std::uint32_t>(this->currentEnemy.speed),
        this->currentEnemy.damage,
        0
    };
    this->enemyHealth.initHP(this->enemyFinalStats);
    this->enemyNameText.setString(
        "Enemy " + std::to_string(this->enemyIndex + 1) + "/" +
        std::to_string(this->enemies.size()) + ": " + this->currentEnemy.nome);
    this->messageText.setString("Choose an ability to attack.");
}

void BattleState::updateBars()
{
    setBar(this->playerHpBar, this->playerHpTrack,
           static_cast<float>(this->playerHealth.getHP()),
           static_cast<float>(this->playerFinalStats.MaxHP));
    setBar(this->manaBar, this->manaTrack,
           static_cast<float>(this->playerMana.CurrentMana),
           static_cast<float>(this->playerFinalStats.MaxMana));

    const float expCurrent = this->progression.podeSubirNivel()
        ? static_cast<float>(this->progression.getExp())
        : 1.f;
    const float expMaximum = this->progression.podeSubirNivel()
        ? static_cast<float>(this->progression.getExpMax())
        : 1.f;
    setBar(this->expBar, this->expTrack, expCurrent, expMaximum);
    setBar(this->enemyHpBar, this->enemyHpTrack,
           static_cast<float>(this->enemyHealth.getHP()),
           static_cast<float>(this->enemyFinalStats.MaxHP));

    this->playerHpText.setString(
        "HP  " + std::to_string(this->playerHealth.getHP()) + "/" +
        std::to_string(this->playerFinalStats.MaxHP));
    this->manaText.setString(
        "Mana  " + std::to_string(this->playerMana.CurrentMana) + "/" +
        std::to_string(this->playerFinalStats.MaxMana));
    this->expText.setString(this->progression.podeSubirNivel()
        ? "XP  " + std::to_string(this->progression.getExp()) + "/" +
            std::to_string(this->progression.getExpMax())
        : "XP  Max level");
    this->levelText.setString(
        "Level " + std::to_string(this->progression.getNivel()) +
        "   |   Next level: " +
        (this->progression.podeSubirNivel()
            ? std::to_string(this->progression.getExpMax()) + " XP"
            : "Max level"));
    this->enemyHpText.setString(
        "HP  " + std::to_string(this->enemyHealth.getHP()) + "/" +
        std::to_string(this->enemyFinalStats.MaxHP));
}

void BattleState::updateInput(const float&)
{
    const auto closeKey = this->keybinds.find("CLOSE");
    if (closeKey != this->keybinds.end() &&
        sf::Keyboard::isKeyPressed(
            static_cast<sf::Keyboard::Scancode>(closeKey->second))) {
        this->returnToWorld();
    }
}

void BattleState::startEncounter(std::size_t selectedEnemyIndex,
                                 std::size_t worldMobIndex)
{
    if (selectedEnemyIndex >= this->enemies.size()) {
        throw std::out_of_range("World encounter enemy index is out of range.");
    }

    if (worldMobIndex >= this->defeatedWorldMobs.size()) {
        throw std::out_of_range("World encounter mob index is out of range.");
    }
    this->enemyIndex = selectedEnemyIndex;
    this->currentWorldMobIndex = worldMobIndex;
    this->gameFinished = false;
    this->loadEnemy();
    this->updateBars();
}

bool BattleState::isWorldMobDefeated(std::size_t worldMobIndex) const
{
    return this->defeatedWorldMobs.at(worldMobIndex);
}

sf::Vector2f BattleState::getWorldPlayerPosition() const
{
    return this->worldPlayerPosition;
}

void BattleState::setWorldPlayerPosition(const sf::Vector2f& position)
{
    this->worldPlayerPosition = position;
}

void BattleState::returnToWorld()
{
    this->states->push(new WorldState(
        this->window, this->supportedKeys, this->states, this->character, *this));
}

void BattleState::openShop()
{
    this->states->push(new ShopState(
        this->window, this->supportedKeys, this->states, this->character,
        this->inventory, this->shopEquipmentChanged, CurrentCoin));
}

void BattleState::update(const float& dt)
{
    if (this->shopEquipmentChanged) {
        this->shopEquipmentChanged = false;
        this->refreshOwnedEquipment();
    }
    this->updateHitFlashes(dt);
    this->updateMousePosition();
    this->updateInput(dt);
    this->updateButtons();
    this->updateBars();
}

void BattleState::flashWhenHit(sf::RectangleShape& shape, float& remaining,
                               float& elapsed)
{
    remaining = hitFlashDuration;
    elapsed = 0.f;
    shape.setFillColor(sf::Color::White);
}

void BattleState::updateHitFlashes(float dt)
{
    const auto updateFlash = [dt](sf::RectangleShape& shape,
                                  const sf::Color& normalColor,
                                  float& remaining, float& elapsed) {
        if (remaining <= 0.f) {
            return;
        }

        const float frameTime = std::max(0.f, dt);
        remaining = std::max(0.f, remaining - frameTime);
        elapsed += frameTime;
        if (remaining <= 0.f) {
            shape.setFillColor(normalColor);
            elapsed = 0.f;
            return;
        }

        const float phase = std::fmod(elapsed, hitFlashInterval * 2.f);
        shape.setFillColor(phase < hitFlashInterval
            ? sf::Color::White
            : normalColor);
    };

    updateFlash(this->playerShape, playerColor,
                this->playerHitFlashRemaining, this->playerHitFlashElapsed);
    updateFlash(this->enemyShape, enemyColor,
                this->enemyHitFlashRemaining, this->enemyHitFlashElapsed);
}

void BattleState::updateButtons()
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

    for (std::size_t index = 0; index < this->moveTable->size(); ++index) {
        if (this->buttons["MOVE_" + std::to_string(index)]->isPressed()) {
            this->actionButtonLatched = true;
            this->useMove(index);
            return;
        }
    }

    if (this->buttons["FLEE"]->isPressed()) {
        this->actionButtonLatched = true;
        this->returnToWorld();
        return;
    }

    if (this->buttons["INVENTORY"]->isPressed()) {
        this->actionButtonLatched = true;
        this->syncEquipmentSelections();
        this->states->push(new InventoryState(
            this->window, this->supportedKeys, this->states, this->inventory,
            this->weapons, this->armors, this->equippedWeapon,
            this->equippedArmor,
            [this]() { this->equipSelectedItems(); },
            [this](bool isPotion, int itemID) {
                if (isPotion) {
                    this->inventory.applyPotion(
                        static_cast<PotionID>(itemID), this->playerFinalStats,
                        this->playerCriticalRate, this->playerHealth,
                        this->playerMana);
                } else {
                    this->inventory.applyConsumable(
                        static_cast<ConsumableID>(itemID),
                        this->playerFinalStats, this->playerCriticalRate,
                        this->playerHealth, this->playerMana);
                }
                this->playerHealth.setShield(this->playerFinalStats.Shield);
                this->updateBars();
            }));
    }
}

void BattleState::useMove(std::size_t moveIndex)
{
    if (this->gameFinished) {
        return;
    }

    const MoveData& move = (*this->moveTable)[moveIndex];
    if (this->progression.getNivel() < move.nivelMinimo) {
        this->messageText.setString(
            "Ability locked: requires level " +
            std::to_string(move.nivelMinimo) + ".");
        return;
    }
    if (move.danoBase > 0 && this->playerMana.CurrentMana == 0) {
        this->messageText.setString(
            "No mana left. You cannot use attacks.");
        return;
    }
    if (move.costMana > this->playerMana.CurrentMana) {
        this->messageText.setString("Not enough mana for this ability.");
        return;
    }

    const bool playerIsFaster =
        this->playerFinalStats.Speed > this->enemyFinalStats.Speed;
    if (!playerIsFaster) {
        this->playEnemyTurn();
        if (!this->playerHealth.isAlive()) {
            this->showDefeatState();
            return;
        }
    }

    this->playerMana.useMana(move.costMana);
    if (move.curaBase > 0) {
        this->playerHealth.getHeal(move.curaBase);
        this->messageText.setString(
            move.nome + " restored " + std::to_string(move.curaBase) + " HP.");
    }
    if (move.danoBase > 0) {
        finalStats attack = this->playerFinalStats;
        attack.Damage += move.danoBase;
        const int previousHp = this->enemyHealth.getHP();
        this->enemyHealth.takeDamage(attack, this->playerCriticalRate);
        const int damage = previousHp - this->enemyHealth.getHP();
        if (damage > 0) {
            this->flashWhenHit(this->enemyShape, this->enemyHitFlashRemaining,
                               this->enemyHitFlashElapsed);
        }
        this->messageText.setString(
            move.nome + " dealt " + std::to_string(damage) +
            " damage to " + this->currentEnemy.nome + ".");
    }

    if (!this->enemyHealth.isAlive()) {
        this->finishEnemy();
        return;
    }
    if (playerIsFaster) {
        this->playEnemyTurn();
        if (!this->playerHealth.isAlive()) {
            this->showDefeatState();
        }
    }
}

void BattleState::showDefeatState()
{
    this->states->push(new DefeatState(
        this->window, this->supportedKeys, this->states));
}

void BattleState::playEnemyTurn()
{
    const int previousHp = this->playerHealth.getHP();
    this->playerHealth.takeDamage(this->enemyFinalStats);
    const int damage = previousHp - this->playerHealth.getHP();
    if (damage > 0) {
        this->flashWhenHit(this->playerShape, this->playerHitFlashRemaining,
                           this->playerHitFlashElapsed);
    }
    this->messageText.setString(
        this->currentEnemy.nome + " attacked and dealt " +
        std::to_string(damage) + " damage.");
    if (this->playerHealth.isAlive()) {
        this->playerMana.getMana(2, this->playerFinalStats.MaxMana);
    }
}

void BattleState::finishEnemy()
{
    const std::string defeatedEnemy = this->currentEnemy.nome;
    const std::uint32_t expReward = this->currentEnemy.expRecompensa;
    const std::uint32_t coinReward = this->currentEnemy.coinRecompensa;
    const std::uint16_t previousLevel = this->progression.getNivel();
    const std::uint16_t levelsGained =
        this->progression.addExp(expReward);

    if (levelsGained > 0) {
        const stats growth = this->levelGrowth();
        for (std::uint16_t level = 0; level < levelsGained; ++level) {
            this->playerBaseStats.VIG += growth.VIG;
            this->playerBaseStats.DEF += growth.DEF;
            this->playerBaseStats.VEL += growth.VEL;
            this->playerBaseStats.FOR += growth.FOR;
            this->playerBaseStats.INT += growth.INT;
        }
        this->recalculatePlayerStats();
    }

    CurrentCoin += static_cast<int>(coinReward);
    this->messageText.setString(
        "You defeated " + defeatedEnemy + " and earned " +
        std::to_string(expReward) + " XP and " +
        std::to_string(coinReward) + " coins." +
        (this->progression.getNivel() > previousLevel
            ? " New level: " + std::to_string(this->progression.getNivel()) + "!"
            : ""));

    this->defeatedWorldMobs.at(this->currentWorldMobIndex) = true;
    this->gameFinished = true;
    this->returnToWorld();
}

void BattleState::drawBar(sf::RenderTarget* target,
                          const sf::RectangleShape& track,
                          const sf::RectangleShape& bar,
                          const sf::Text& label)
{
    target->draw(track);
    target->draw(bar);
    target->draw(label);
}

void BattleState::render(sf::RenderTarget* target)
{
    if (!target) {
        target = this->window;
    }

    target->draw(this->background);
    target->draw(this->hudPanel);
    target->draw(this->arenaPanel);
    target->draw(this->playerNameText);
    target->draw(this->playerStatsText);
    target->draw(this->levelText);
    target->draw(this->enemyNameText);
    this->drawBar(target, this->playerHpTrack, this->playerHpBar,
                  this->playerHpText);
    this->drawBar(target, this->manaTrack, this->manaBar, this->manaText);
    this->drawBar(target, this->expTrack, this->expBar, this->expText);
    this->drawBar(target, this->enemyHpTrack, this->enemyHpBar,
                  this->enemyHpText);
    target->draw(this->playerShape);
    target->draw(this->enemyShape);
    target->draw(this->messageText);

    for (auto& button : this->buttons) {
        button.second->render(target);
    }
}
