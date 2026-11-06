#pragma once

#include "Armaments.hpp"
#include "Armors.hpp"

#include <string>
#include <vector>

struct EquipmentOption {
    std::string name;
    stats bonuses;
};

template <typename Equipment, typename ItemId>
std::vector<EquipmentOption> getEquipmentOptions(
    const Equipment& equipment)
{
    std::vector<EquipmentOption> options = {
        {"None", {0, 0, 0, 0, 0}}
    };

    for (int id = 1; id <= 3; ++id) {
        std::string name;
        stats bonuses{};
        if (equipment.getItemData(static_cast<ItemId>(id), name, bonuses)) {
            options.push_back({name, bonuses});
        }
    }
    return options;
}

inline std::vector<EquipmentOption> getWeaponsForClass(
    const std::string& characterClass)
{
    if (characterClass == "Hunter") {
        return getEquipmentOptions<hunterArmament, hunter_ArmamentID>(
            hunterArmament{});
    }
    if (characterClass == "Mage") {
        return getEquipmentOptions<mageArmament, mage_ArmamentID>(
            mageArmament{});
    }
    if (characterClass == "Cleric") {
        return getEquipmentOptions<clericArmament, cleric_ArmamentID>(
            clericArmament{});
    }
    return getEquipmentOptions<warriorArmament, warrior_ArmamentID>(
        warriorArmament{});
}

inline std::vector<EquipmentOption> getArmorForClass(
    const std::string& characterClass)
{
    if (characterClass == "Hunter") {
        return getEquipmentOptions<Arrow_armor, Arrow_armorID>(
            Arrow_armor{});
    }
    if (characterClass == "Mage") {
        return getEquipmentOptions<Wizard_armor, Wizard_armorID>(
            Wizard_armor{});
    }
    if (characterClass == "Cleric") {
        return getEquipmentOptions<Cleric_armor, Cleric_armorID>(
            Cleric_armor{});
    }
    return getEquipmentOptions<Warrior_armor, Warrior_armorID>(
        Warrior_armor{});
}
