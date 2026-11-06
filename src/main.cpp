#include <iostream>
#include <string>
#include <ctime>

#include "../library/Systems/Itens/Effects.hpp"
#include "../library/character/Classes/allClasses.hpp"
#include "../library/character/Races/allRaces.hpp"
#include "../library/Systems/World/Treasures/Chests.hpp"
#include "../library/Systems/Economy/Market/shop.hpp"
#include "../library/Battle.hpp"

#include "../Header/Game.hpp"


/* definição das variáveis globais declaradas em Stats_point.hpp */
stats    globalStats    = {0, 0, 0, 0, 0};
raceStats globalRaceStats = {0, 0, 0, 0, 0};
effectsBonus eff;

/* ═══════════════════════════════════════════════════
   Seleção de classe
═══════════════════════════════════════════════════ */

void printSep() {
    std::cout << "----------------------------------------\n";
}

int escolherClasse() {
    printSep();
    std::cout << "  CHOOSE YOUR CLASS\n\n";
    std::cout << "  [1] WARRIOR          • High defense and strength - Ideal for direct combat\n";
    std::cout << "  [2] HUNTER            • Balanced speed and strength - Fast attacks\n";
    std::cout << "  [3] MAGE              • High intelligence - Long-range magic and damage\n";
    std::cout << "  [4] CLERIC            • High vitality - Can heal and support allies\n";
    printSep();
    std::cout << "  What is your choice, Adventurer? > ";

    int op;
    std::cin >> op;
    return op;
}   

int escolherRaca(){
    printSep();
    std::cout << "  CHOOSE YOUR RACE \n\n";
    std::cout << "  [1] HUMAN   • Versatile and balanced - Great for beginners\n";
    std::cout << "  [2] ELF     • Agile and magical - Speed and intelligence\n";
    std::cout << "  [3] DWARF   • Strong and resilient - Defense and vitality\n";
    printSep();
    std::cout << "  Which race do you choose? > ";

    int op;
    std::cin >> op;
    return op;
}

/* ═══════════════════════════════════════════════════
   Main
═══════════════════════════════════════════════════ */

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    Game game;
    game.run();
/*
    printSep();
    std::cout <<
    "\n"
    "  ███████╗████████╗██╗  ██╗███████╗██████╗ ███████╗ █████╗ ██╗     \n" <<
    "  ██╔════╝╚══██╔══╝██║  ██║██╔════╝██╔══██╗██╔════╝██╔══██╗██║     \n" <<
    "  █████╗     ██║   ███████║█████╗  ██████╔╝█████╗  ███████║██║     \n" <<
    "  ██╔══╝     ██║   ██╔══██║██╔══╝  ██╔══██╗██╔══╝  ██╔══██║██║     \n" <<
    "  ███████╗   ██║   ██║  ██║███████╗██║  ██║███████╗██║  ██║███████╗\n" <<
    "  ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝\n" <<
    "\n" <<
    "  ██████╗ ██╗███████╗████████╗\n" <<
    "  ██╔══██╗██║██╔════╝╚══██╔══╝\n" <<
    "  ██████╔╝██║█████╗     ██║   \n" <<
    "  ██╔══██╗██║██╔══╝     ██║   \n" <<
    "  ██║  ██║██║██║        ██║   \n" <<
    "  ╚═╝  ╚═╝╚═╝╚═╝        ╚═╝   \n"<<
    "\n";
    printSep();

    // nome do personagem 
    std::string nome;
    std::cout << "\n  What is your name, traveler? > ";
    std::getline(std::cin, nome);
    if (nome.empty()) nome = "Hero";

    // Escolha das Raças
    int raca = escolherRaca();

    switch(raca){
        case 1: {
            Human humano;
            std::cout << "\n  Choose your human lineage:\n\n";
            std::cout << "  [1]  Jungle Human      • Balanced with a strength bonus\n";
            std::cout << "  [2]  Mountain Human    • Balanced with a vitality bonus\n";
            std::cout << "  > ";
            int subRaca;
            std::cin >> subRaca;
            if(subRaca == 1) {
                humano.beRace(id_subHuman::JUNGLE_HUMAN);
                std::cout << "   You are a Jungle Human! Prepare for adventure...\n";
            }
            else if(subRaca == 2) {
                humano.beRace(id_subHuman::MOUNTAIN_HUMAN);
                std::cout << "   You are a Mountain Human! Your endurance is unmatched.\n";
            }
            else {
                std::cout << "   Invalid choice! Defaulting to Jungle Human.\n";
                humano.beRace(id_subHuman::JUNGLE_HUMAN);
            }

            globalRaceStats = humano.getRaceStats();
            break;
        }

        case 2:{
            Elf elfo;
            std::cout << "\n  Choose your elf lineage:\n\n";
            std::cout << "  [1]  Blood Elf       • High vitality and intelligence\n";
            std::cout << "  [2]  Mountain Elf    • Superior speed and strength\n";
            std::cout << "  [3]  Snow Elf        • Improved endurance and speed\n";
            std::cout << "  > ";
            int subRaca;
            std::cin >> subRaca;
            if(subRaca == 1) {
                elfo.beRace(id_subElf::BOOD_ELF);
                std::cout << "   You are a Blood Elf! Ancestral power flows through your veins.\n";
            }
            else if(subRaca == 2) {
                elfo.beRace(id_subElf::MOUNTAIN_ELF);
                std::cout << "   You are a Mountain Elf! Speed is your ally.\n";
            }
            else if(subRaca == 3) {
                elfo.beRace(id_subElf::SNOW_ELF);
                std::cout << "   You are a Snow Elf! Cold and grace define you.\n";
            }
            else {
                std::cout << "   Invalid choice! Defaulting to Blood Elf.\n";
                elfo.beRace(id_subElf::BOOD_ELF);   
            }

            globalRaceStats = elfo.getRaceStats();

            break;
        }

        case 3: {
            Dwarf anao;
            std::cout << "\n  Choose your dwarf lineage:\n\n";
            std::cout << "  [1]  Forge Dwarf        • Superior vitality and defense\n";
            std::cout << "  [2]  Hill Dwarf         • Increased defense and strength\n";
            std::cout << "  [3]  Underground Dwarf  • Deep-rooted vitality and strength\n";
            std::cout << "  > ";
            int subRaca;
            std::cin >> subRaca;
            if(subRaca == 1) {
                anao.beRace(id_subDwarf::FORGE_DWARF);
                std::cout << "   You are a Forge Dwarf! Your muscles can break mountains.\n";
            }
            else if(subRaca == 2) {
                anao.beRace(id_subDwarf::COULINS_DWARF);
                std::cout << "   You are a Hill Dwarf! Unbreakable defense is your hallmark.\n";
            }
            else if(subRaca == 3) {
                anao.beRace(id_subDwarf::UNDERGROUND_DWARF);
                std::cout << "   You are an Underground Dwarf! Primal power runs through your veins.\n";
            }
            else {
                std::cout << "   Invalid choice! Defaulting to Forge Dwarf.\n";
                anao.beRace(id_subDwarf::FORGE_DWARF);
            }
            globalRaceStats = anao.getRaceStats();
            break;
        }
    }

    *//* escolha da classe */
    //int classe = escolherClasse();
    
    /* inicia a arena com a classe escolhida */
    /*
    switch (classe) {
        case 1: {
            Warrior guerreiro;
            guerreiro.equipar(warrior_ArmamentID::GRASS_BLADE);
            arena(guerreiro, nome);
            break;
        }
        case 2: {
            Hunter cacador;
            cacador.equipar(hunter_ArmamentID::WOOD_BOW);
            arena(cacador, nome);
            break;
        }
        case 3: {
            Mage mago;
            mago.equipar(mage_ArmamentID::WOOD_STAFF);
            arena(mago, nome);
            break;
        }
        case 4: {
            Cleric clerigo;
            clerigo.equipar(cleric_ArmamentID::MACE);
            arena(clerigo, nome);
            break;
        }
        default: {
            std::cout << "   Invalid class! You will awaken as a Warrior...\n";
            Warrior guerreiro;
            arena(guerreiro, nome);
            break;
        }
    }
   */
    return 0;
}