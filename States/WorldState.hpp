#pragma once

#include "State.hpp"
#include "../Header/CharacterConfig.hpp"

#include <cstddef>
#include <random>
#include <vector>

class BattleState;

class WorldState : public State
{
public:
    WorldState(sf::RenderWindow* window,
               std::map<std::string, int>* supportedKeys,
               std::stack<State*>* states,
               const CharacterConfig& character,
               BattleState& battle);

    void update(const float& dt) override;
    void updateInput(const float& dt) override;
    void render(sf::RenderTarget* target = NULL) override;

private:
    struct Mob {
        std::size_t enemyIndex;
        sf::RectangleShape hitbox;
        sf::Text label;
        sf::Vector2f direction;
        float speed;
        float directionChangeRemaining;

        Mob(const sf::Font& font, std::size_t enemyIndex,
            const std::string& name, const sf::Vector2f& position,
            const sf::Vector2f& direction, float speed,
            float directionChangeRemaining);
    };

    enum class InteractionType {
        None,
        Mob,
        Merchant
    };

    struct NearbyInteraction {
        InteractionType type = InteractionType::None;
        std::size_t mobIndex = 0;
    };

    CharacterConfig character;
    BattleState& battle;
    std::mt19937 randomEngine{std::random_device{}()};
    sf::Font font;
    sf::RectangleShape background;
    sf::RectangleShape mapArea;
    sf::RectangleShape path;
    sf::RectangleShape playerHitbox;
    sf::RectangleShape merchantHitbox;
    sf::Text titleText;
    sf::Text instructionsText;
    sf::Text playerLabel;
    sf::Text merchantLabel;
    sf::Text promptText;
    sf::Vector2f merchantDirection;
    float merchantSpeed = 0.f;
    float merchantDirectionChangeRemaining = 0.f;
    std::vector<Mob> mobs;
    bool interactLatched = false;

    void initKeybinds();
    void initWorld();
    void updateMobs(float dt);
    void updatePrompt();
    NearbyInteraction findNearbyInteraction() const;
    void interactWithNearbyCharacter();
};
