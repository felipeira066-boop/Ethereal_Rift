#pragma once

#include <array>
#include <string>

struct CharacterConfig {
    std::string name = "Hero";
    std::string race = "Human";
    std::string characterClass = "Warrior";
    std::array<float, 5> stats = {2.f, 2.f, 2.f, 3.f, 2.f};
};
