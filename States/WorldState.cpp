#include "WorldState.hpp"

#include "BattleState.hpp"
#include "../Header/Palette.hpp"
#include "../library/Arena.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace {
constexpr float worldLeft = 48.f;
constexpr float worldTop = 148.f;
constexpr float worldRight = 1232.f;
constexpr float worldBottom = 640.f;
constexpr float interactionDistance = 88.f;

struct MobSpawn {
    std::size_t enemyIndex;
    sf::Vector2f position;
};

const sf::Vector2f mobPositions[] = {
    {360.f, 250.f},
    {735.f, 215.f},
    {1010.f, 330.f},
    {500.f, 480.f},
    {815.f, 500.f},
    {1100.f, 215.f}
};
}

WorldState::Mob::Mob(const sf::Font& font, std::size_t enemyIndex,
                     const std::string& name, int recommendedLevel,
                     const sf::Vector2f& position,
                     const sf::Vector2f& direction, float speed,
                     float directionChangeRemaining)
    : enemyIndex(enemyIndex),
      hitbox(sf::Vector2f(38.f, 44.f)),
      label(font, name, 14),
      levelLabel(font, "Lv. " + std::to_string(recommendedLevel), 12),
      direction(direction),
      speed(speed),
      directionChangeRemaining(directionChangeRemaining)
{
    this->hitbox.setPosition(position);
    this->hitbox.setFillColor(sf::Color(
        theme::russet.r, theme::russet.g, theme::russet.b, 170));
    this->hitbox.setOutlineColor(theme::alloyOrange);
    this->hitbox.setOutlineThickness(2.f);
    this->label.setFillColor(theme::cornsilk);
    this->label.setPosition(sf::Vector2f(position.x - 20.f, position.y - 28.f));
    this->levelLabel.setFillColor(theme::lemonMeringue);
    this->levelLabel.setPosition(sf::Vector2f(position.x - 20.f, position.y - 13.f));
}

WorldState::WorldState(sf::RenderWindow* window,
                       std::map<std::string, int>* supportedKeys,
                       std::stack<State*>* states,
                       const CharacterConfig& character,
                       BattleState& battle)
    : State(window, supportedKeys, states),
      character(character),
      battle(battle),
      titleText(this->font, "OPEN WORLD", 24),
      instructionsText(this->font, "Move: WASD    |    Interact: E    |    Exit: Esc", 16),
      playerLabel(this->font, this->character.name, 14),
      merchantLabel(this->font, "Merchant", 16),
      promptText(this->font, "", 18)
{
    if (!this->font.openFromFile("Fonts/slkscr.ttf")) {
        throw "ERROR::WORLD_STATE::INIT_FONT";
    }

    this->initKeybinds();
    this->initWorld();
}

void WorldState::initKeybinds()
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

void WorldState::initWorld()
{
    this->background.setSize(sf::Vector2f(1280.f, 720.f));
    this->background.setFillColor(theme::darkOliveGreen);

    this->mapArea.setPosition(sf::Vector2f(32.f, 112.f));
    this->mapArea.setSize(sf::Vector2f(1216.f, 580.f));
    this->mapArea.setFillColor(theme::laurelGreen);
    this->mapArea.setOutlineColor(theme::camel);
    this->mapArea.setOutlineThickness(3.f);

    this->path.setPosition(sf::Vector2f(33.f, 365.f));
    this->path.setSize(sf::Vector2f(1214.f, 60.f));
    this->path.setFillColor(theme::darkOliveGreen);

    this->titleText.setFillColor(theme::cornsilk);
    this->titleText.setPosition(sf::Vector2f(48.f, 20.f));
    this->instructionsText.setFillColor(theme::lemonMeringue);
    this->instructionsText.setPosition(sf::Vector2f(48.f, 62.f));

    this->playerHitbox.setSize(sf::Vector2f(32.f, 42.f));
    this->playerHitbox.setFillColor(sf::Color(
        theme::darkOliveGreen.r, theme::darkOliveGreen.g,
        theme::darkOliveGreen.b, 120));
    this->playerHitbox.setOutlineColor(theme::lemonMeringue);
    this->playerHitbox.setOutlineThickness(3.f);
    this->playerHitbox.setPosition(this->battle.getWorldPlayerPosition());

    this->merchantHitbox.setSize(sf::Vector2f(40.f, 46.f));
    this->merchantHitbox.setFillColor(sf::Color(
        theme::camel.r, theme::camel.g, theme::camel.b, 190));
    this->merchantHitbox.setOutlineColor(theme::lemonMeringue);
    this->merchantHitbox.setOutlineThickness(2.f);
    this->merchantHitbox.setPosition(sf::Vector2f(620.f, 355.f));

    this->playerLabel.setFillColor(theme::cornsilk);
    this->merchantLabel.setFillColor(theme::cornsilk);
    this->promptText.setFillColor(theme::cornsilk);
    this->promptText.setPosition(sf::Vector2f(48.f, 652.f));

    std::uniform_real_distribution<float> angleDistribution(
        0.f, 6.28318530718f);
    std::uniform_real_distribution<float> speedDistribution(45.f, 85.f);
    std::uniform_real_distribution<float> merchantSpeedDistribution(35.f, 60.f);
    std::uniform_real_distribution<float> directionDurationDistribution(
        0.8f, 2.4f);

    const int playerLevel = this->battle.getPlayerLevel();
    const int preferredMinimum = std::max(1, playerLevel - 1);
    const int maximumLevel = playerLevel + 1;
    std::vector<std::size_t> eligibleEnemies;
    for (std::size_t index = 0; index < arenaInimigos.size(); ++index) {
        const int recommendedLevel = arenaInimigos[index].nivelRecomendado;
        if (recommendedLevel >= preferredMinimum &&
            recommendedLevel <= maximumLevel) {
            eligibleEnemies.push_back(index);
        }
    }

    if (eligibleEnemies.size() < std::size(mobPositions) / 2) {
        eligibleEnemies.clear();
        const int safeMinimum = std::max(1, playerLevel - 3);
        for (std::size_t index = 0; index < arenaInimigos.size(); ++index) {
            const int recommendedLevel = arenaInimigos[index].nivelRecomendado;
            if (recommendedLevel >= safeMinimum &&
                recommendedLevel <= maximumLevel) {
                eligibleEnemies.push_back(index);
            }
        }
    }

    if (eligibleEnemies.empty()) {
        throw std::runtime_error("No monsters are available for the player's level.");
    }

    std::shuffle(eligibleEnemies.begin(), eligibleEnemies.end(),
                 this->randomEngine);
    this->mobs.reserve(std::size(mobPositions));
    for (std::size_t index = 0; index < std::size(mobPositions); ++index) {
        const float angle = angleDistribution(this->randomEngine);
        const std::size_t enemyIndex =
            eligibleEnemies[index % eligibleEnemies.size()];
        const EnemyData& enemy = arenaInimigos.at(enemyIndex);
        this->mobs.emplace_back(
            this->font, enemyIndex, enemy.nome, enemy.nivelRecomendado,
            mobPositions[index],
            sf::Vector2f(std::cos(angle), std::sin(angle)),
            speedDistribution(this->randomEngine),
            directionDurationDistribution(this->randomEngine));
    }

    const float merchantAngle = angleDistribution(this->randomEngine);
    this->merchantDirection =
        sf::Vector2f(std::cos(merchantAngle), std::sin(merchantAngle));
    this->merchantSpeed = merchantSpeedDistribution(this->randomEngine);
    this->merchantDirectionChangeRemaining =
        directionDurationDistribution(this->randomEngine);
    this->merchantLabel.setPosition(sf::Vector2f(614.f, 333.f));
}

void WorldState::updateMobs(float dt)
{
    const float elapsed = std::clamp(dt, 0.f, 0.1f);
    if (elapsed == 0.f) {
        return;
    }

    std::uniform_real_distribution<float> angleDistribution(
        0.f, 6.28318530718f);
    std::uniform_real_distribution<float> directionDurationDistribution(
        0.8f, 2.4f);

    const auto moveActor = [this, elapsed, &angleDistribution,
                            &directionDurationDistribution](
        sf::RectangleShape& hitbox, sf::Text& label,
        sf::Vector2f& direction, float speed, float& directionRemaining) {
        directionRemaining -= elapsed;
        if (directionRemaining <= 0.f) {
            const float angle = angleDistribution(this->randomEngine);
            direction = sf::Vector2f(std::cos(angle), std::sin(angle));
            directionRemaining =
                directionDurationDistribution(this->randomEngine);
        }

        sf::Vector2f position =
            hitbox.getPosition() + direction * (speed * elapsed);
        const float maxX = worldRight - hitbox.getSize().x;
        const float maxY = worldBottom - hitbox.getSize().y;

        if (position.x < worldLeft) {
            position.x = worldLeft;
            direction.x = std::abs(direction.x);
        } else if (position.x > maxX) {
            position.x = maxX;
            direction.x = -std::abs(direction.x);
        }
        if (position.y < worldTop) {
            position.y = worldTop;
            direction.y = std::abs(direction.y);
        } else if (position.y > maxY) {
            position.y = maxY;
            direction.y = -std::abs(direction.y);
        }

        hitbox.setPosition(position);
        label.setPosition(sf::Vector2f(position.x - 20.f, position.y - 28.f));
    };

    for (std::size_t index = 0; index < this->mobs.size(); ++index) {
        if (this->battle.isWorldMobDefeated(index)) {
            continue;
        }

        Mob& mob = this->mobs[index];
        moveActor(mob.hitbox, mob.label, mob.direction, mob.speed,
                  mob.directionChangeRemaining);
        mob.levelLabel.setPosition(sf::Vector2f(
            mob.hitbox.getPosition().x - 20.f,
            mob.hitbox.getPosition().y - 13.f));
    }

    moveActor(this->merchantHitbox, this->merchantLabel,
              this->merchantDirection, this->merchantSpeed,
              this->merchantDirectionChangeRemaining);
}

WorldState::NearbyInteraction WorldState::findNearbyInteraction() const
{
    const sf::Vector2f playerCenter(
        this->playerHitbox.getPosition().x + this->playerHitbox.getSize().x / 2.f,
        this->playerHitbox.getPosition().y + this->playerHitbox.getSize().y / 2.f);
    float nearestDistance = interactionDistance;
    NearbyInteraction nearest;

    for (std::size_t index = 0; index < this->mobs.size(); ++index) {
        const Mob& mob = this->mobs[index];
        if (this->battle.isWorldMobDefeated(index)) {
            continue;
        }

        const sf::Vector2f mobCenter(
            mob.hitbox.getPosition().x + mob.hitbox.getSize().x / 2.f,
            mob.hitbox.getPosition().y + mob.hitbox.getSize().y / 2.f);
        const float distance = std::hypot(
            playerCenter.x - mobCenter.x, playerCenter.y - mobCenter.y);
        if (distance <= nearestDistance) {
            nearest.type = InteractionType::Mob;
            nearest.mobIndex = index;
            nearestDistance = distance;
        }
    }

    const sf::Vector2f merchantCenter(
        this->merchantHitbox.getPosition().x + this->merchantHitbox.getSize().x / 2.f,
        this->merchantHitbox.getPosition().y + this->merchantHitbox.getSize().y / 2.f);
    const float merchantDistance = std::hypot(
        playerCenter.x - merchantCenter.x, playerCenter.y - merchantCenter.y);
    if (merchantDistance <= nearestDistance) {
        nearest.type = InteractionType::Merchant;
    }

    return nearest;
}

void WorldState::updatePrompt()
{
    const NearbyInteraction nearby = this->findNearbyInteraction();
    if (nearby.type == InteractionType::Merchant) {
        this->promptText.setString("Press E to trade with the Merchant.");
    } else if (nearby.type == InteractionType::Mob) {
        const Mob& mob = this->mobs.at(nearby.mobIndex);
        this->promptText.setString(
            "Press E to fight " + arenaInimigos.at(mob.enemyIndex).nome +
            " (Lv. " +
            std::to_string(arenaInimigos.at(mob.enemyIndex).nivelRecomendado) +
            ").");
    } else {
        this->promptText.setString(
            "Explore and approach a monster or the Merchant.");
    }

    this->playerLabel.setPosition(sf::Vector2f(
        this->playerHitbox.getPosition().x - 6.f,
        this->playerHitbox.getPosition().y - 22.f));
    this->merchantLabel.setPosition(sf::Vector2f(
        this->merchantHitbox.getPosition().x - 20.f,
        this->merchantHitbox.getPosition().y - 22.f));
}

void WorldState::interactWithNearbyCharacter()
{
    const NearbyInteraction nearby = this->findNearbyInteraction();
    if (nearby.type == InteractionType::Merchant) {
        this->battle.openShop();
    } else if (nearby.type == InteractionType::Mob) {
        const Mob& mob = this->mobs.at(nearby.mobIndex);
        this->battle.startEncounter(mob.enemyIndex, nearby.mobIndex);
        this->endState();
    }
}

void WorldState::updateInput(const float& dt)
{
    const auto isActionPressed = [this](const char* action) {
        const auto key = this->keybinds.find(action);
        return key != this->keybinds.end() &&
            sf::Keyboard::isKeyPressed(
                static_cast<sf::Keyboard::Scancode>(key->second));
    };

    sf::Vector2f direction;
    if (isActionPressed("MOVE_LEFT")) {
        direction.x -= 1.f;
    }
    if (isActionPressed("MOVE_RIGHT")) {
        direction.x += 1.f;
    }
    if (isActionPressed("MOVE_UP")) {
        direction.y -= 1.f;
    }
    if (isActionPressed("MOVE_DOWN")) {
        direction.y += 1.f;
    }

    const float length = std::hypot(direction.x, direction.y);
    if (length > 0.f) {
        const float elapsed = std::clamp(dt, 0.f, 0.1f);
        direction.x /= length;
        direction.y /= length;
        this->playerHitbox.move(direction * (240.f * elapsed));
        sf::Vector2f position = this->playerHitbox.getPosition();
        position.x = std::clamp(position.x, worldLeft, worldRight - 32.f);
        position.y = std::clamp(position.y, worldTop, worldBottom - 42.f);
        this->playerHitbox.setPosition(position);
        this->battle.setWorldPlayerPosition(position);
    }

    const bool interactPressed = isActionPressed("INTERACT");
    if (!interactPressed) {
        this->interactLatched = false;
    } else if (!this->interactLatched) {
        this->interactLatched = true;
        this->interactWithNearbyCharacter();
    }

    if (isActionPressed("CLOSE")) {
        this->battle.endState();
        this->endState();
    }
}

void WorldState::update(const float& dt)
{
    this->updateInput(dt);
    this->updateMobs(dt);
    this->updatePrompt();
}

void WorldState::render(sf::RenderTarget* target)
{
    if (!target) {
        target = this->window;
    }

    target->draw(this->background);
    target->draw(this->mapArea);
    target->draw(this->path);
    target->draw(this->titleText);
    target->draw(this->instructionsText);

    for (std::size_t index = 0; index < this->mobs.size(); ++index) {
        if (this->battle.isWorldMobDefeated(index)) {
            continue;
        }
        target->draw(this->mobs[index].hitbox);
        target->draw(this->mobs[index].label);
        target->draw(this->mobs[index].levelLabel);
    }

    target->draw(this->merchantHitbox);
    target->draw(this->merchantLabel);
    target->draw(this->playerHitbox);
    target->draw(this->playerLabel);
    target->draw(this->promptText);
}
