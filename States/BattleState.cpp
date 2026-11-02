#include "BattleState.hpp"
#include "InventoryState.hpp"
#include "Header/Palette.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

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
}

BattleState::BattleState(sf::RenderWindow* window,
                         std::map<std::string, int>* supportedKeys,
                         std::stack<State*>* states,
                         const CharacterConfig& character)
    : State(window, supportedKeys, states),
      character(character),
      playerNameText(this->font, "", 22),
      playerStatsText(this->font, "", 16),
      playerHpText(this->font, "", 16),
      manaText(this->font, "", 16),
      expText(this->font, "", 16),
      levelText(this->font, "", 18),
      enemyNameText(this->font, "", 22),
      enemyHpText(this->font, "", 16),
      messageText(this->font, "", 20),
      enemies(arenaInimigos)
{
    if (!this->font.openFromFile("Fonts/GeistPixel-Regular-VariableFont_ELSH.ttf")) {
        throw "ERROR::BATTLE_STATE::INIT_FONT";
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
        return {1, 1, 2, 2, 1};
    }
    if (this->character.characterClass == "Cleric") {
        return {2, 1, 1, 1, 2};
    }
    return {2, 1, 1, 2, 1};
}

void BattleState::recalculatePlayerStats()
{
    // CharacterConfig already combines race bonuses with class base stats.
    this->playerFinalStats = totalStats::contagem(
        this->playerBaseStats, raceStats{0, 0, 0, 0, 0});
    this->playerHealth.initHP(this->playerFinalStats);
    this->playerMana.initMana(this->playerFinalStats);

    std::stringstream statsText;
    statsText << "VIG " << this->playerBaseStats.VIG
              << "   DEF " << this->playerBaseStats.DEF
              << "   VEL " << this->playerBaseStats.VEL
              << "   FOR " << this->playerBaseStats.FOR
              << "   INT " << this->playerBaseStats.INT;
    this->playerStatsText.setString(statsText.str());
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
    this->messageText.setString("Escolha uma habilidade para iniciar o turno.");

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
        844.f, buttonY, 190.f, 56.f, &this->font, "Sair",
        theme::russet, theme::alloyOrange, theme::camel);
    this->buttons["INVENTORY"] = new Button(
        1050.f, buttonY, 190.f, 56.f, &this->font, "Inventario",
        theme::darkOliveGreen, theme::laurelGreen, theme::camel);
    this->playerNameText.setString(
        this->character.name + " - " + this->character.race + " "
        + this->character.characterClass);
}

void BattleState::loadEnemy()
{
    if (this->enemyIndex >= this->enemies.size()) {
        this->gameFinished = true;
        this->messageText.setString("Arena concluida! Pressione Sair para retornar.");
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
        "Inimigo " + std::to_string(this->enemyIndex + 1) + "/" +
        std::to_string(this->enemies.size()) + ": " + this->currentEnemy.nome);
    this->messageText.setString("Escolha uma habilidade para atacar.");
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
        : "XP  Nivel maximo");
    this->levelText.setString(
        "Nivel " + std::to_string(this->progression.getNivel()) +
        "   |   Proximo nivel: " +
        (this->progression.podeSubirNivel()
            ? std::to_string(this->progression.getExpMax()) + " XP"
            : "nivel maximo"));
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
        this->endState();
    }
}

void BattleState::update(const float& dt)
{
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
        this->endState();
        return;
    }

    if (this->buttons["INVENTORY"]->isPressed()) {
        this->actionButtonLatched = true;
        this->states->push(new InventoryState(
            this->window, this->supportedKeys, this->states, this->inventory));
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
            "Habilidade bloqueada: requer nivel " +
            std::to_string(move.nivelMinimo) + ".");
        return;
    }
    if (move.danoBase > 0 && this->playerMana.CurrentMana == 0) {
        this->messageText.setString(
            "Mana zerada. Voce nao pode usar ataques.");
        return;
    }
    if (move.costMana > this->playerMana.CurrentMana) {
        this->messageText.setString("Mana insuficiente para essa habilidade.");
        return;
    }

    const bool playerIsFaster =
        this->playerFinalStats.Speed > this->enemyFinalStats.Speed;
    if (!playerIsFaster) {
        this->playEnemyTurn();
        if (!this->playerHealth.isAlive()) {
            this->returnToMainMenu();
            return;
        }
    }

    this->playerMana.useMana(move.costMana);
    if (move.curaBase > 0) {
        this->playerHealth.getHeal(move.curaBase);
        this->messageText.setString(
            move.nome + " restaurou " + std::to_string(move.curaBase) + " HP.");
    }
    if (move.danoBase > 0) {
        finalStats attack = this->playerFinalStats;
        attack.Damage += move.danoBase;
        const int previousHp = this->enemyHealth.getHP();
        this->enemyHealth.takeDamage(attack);
        const int damage = previousHp - this->enemyHealth.getHP();
        if (damage > 0) {
            this->flashWhenHit(this->enemyShape, this->enemyHitFlashRemaining,
                               this->enemyHitFlashElapsed);
        }
        this->messageText.setString(
            move.nome + " causou " + std::to_string(damage) +
            " de dano em " + this->currentEnemy.nome + ".");
    }

    if (!this->enemyHealth.isAlive()) {
        this->finishEnemy();
        return;
    }
    if (playerIsFaster) {
        this->playEnemyTurn();
        if (!this->playerHealth.isAlive()) {
            this->returnToMainMenu();
        }
    }
}

void BattleState::returnToMainMenu()
{
    State* battleState = this->states->top();
    this->states->pop();
    while (this->states->size() > 1) {
        delete this->states->top();
        this->states->pop();
    }
    this->states->push(battleState);
    this->endState();
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
        this->currentEnemy.nome + " atacou e causou " +
        std::to_string(damage) + " de dano.");
    if (this->playerHealth.isAlive()) {
        this->playerMana.getMana(2, this->playerFinalStats.MaxMana);
    }
}

void BattleState::finishEnemy()
{
    const std::string defeatedEnemy = this->currentEnemy.nome;
    const std::uint32_t expReward = this->currentEnemy.expRecompensa;
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

    ++this->enemyIndex;
    this->loadEnemy();
    this->messageText.setString(
        "Voce derrotou " + defeatedEnemy + " e ganhou " +
        std::to_string(expReward) + " XP." +
        (this->progression.getNivel() > previousLevel
            ? " Novo nivel: " + std::to_string(this->progression.getNivel()) + "!"
            : "") +
        (this->gameFinished ? " Arena concluida!" : ""));
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
