#include <iostream>
#include <string>
#include "src/bibliotecas.hpp"
#include "library/assets/sounds/background.hpp"

/* definição das variáveis globais declaradas em Stats_point.hpp */
stats    globalStats    = {0, 0, 0, 0, 0};
raceStats globalRaceStats = {0, 0, 0, 0, 0};

/* ═══════════════════════════════════════════════════
   Seleção de classe
═══════════════════════════════════════════════════ */

void printSep() {
    std::cout << "----------------------------------------\n";
}

int escolherClasse() {
    printSep();
    std::cout << "  Escolha sua classe:\n\n";
    std::cout << "  1. Guerreiro  (DEF alta | FOR altos)\n";
    std::cout << "  2. Caçador    (VEL alta | FOR media)\n";
    std::cout << "  3. Mago       (INT alta | VIG baixo)\n";
    std::cout << "  4. Clérigo    (VIG alto | pode curar)\n";
    printSep();
    std::cout << "> ";

    int op;
    std::cin >> op;
    return op;
}   

int escolherRaca(){
    printSep();
    std::cout << "  Escolha sua raça:\n\n";
    std::cout << "  1. Humano\n";
    std::cout << "  2. Elfo\n";
    std::cout << "  3. Anão\n";
    printSep();
    std::cout << "> ";

    int op;
    std::cin >> op;
    return op;
}

/* ═══════════════════════════════════════════════════
   Main
═══════════════════════════════════════════════════ */

int main() {
    srand(time(NULL));

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

    /* nome do personagem */
    std::string nome;
    std::cout << "\n  Nome do personagem: ";
    std::getline(std::cin, nome);
    if (nome.empty()) nome = "Heroi";

    /* Escolha das Raças */
    int raca = escolherRaca();

    switch(raca){
        case 1: {
            Human humano;
            std::cout << "  Qual sub-raça de humano você quer?\n";
            std::cout << "  1. Humano da Selva (equilibrado + FOR)\n";
            std::cout << "  2. Humano das Montanhas (equilibrado + VIG)\n";
            std::cout << "  > ";
            int subRaca;
            std::cin >> subRaca;
            if(subRaca == 1) humano.beRace(id_subHuman::JUNGLE_HUMAN);
            else if(subRaca == 2) humano.beRace(id_subHuman::MOUNTAIN_HUMAN);
            else {
                std::cout << "  Sub-raça inválida, usando Humano da Selva.\n";
                humano.beRace(id_subHuman::JUNGLE_HUMAN);
            }

            globalRaceStats = humano.getRaceStats();
            break;
        }

        case 2:{
            Elf elfo;
            std::cout << "  Qual sub-raça de elfo você quer?\n";
            std::cout << "  1. Elfo de Sangue (bônus em VIG e INT)\n";
            std::cout << "  2. Elfo das Montanhas (bônus em VEL e FOR)\n";
            std::cout << "  3. Elfo das Neves (bônus em VIG e VEL)\n";
            std::cout << "  > ";
            int subRaca;
            std::cin >> subRaca;
            if(subRaca == 1) elfo.beRace(id_subElf::BOOD_ELF);
            else if(subRaca == 2) elfo.beRace(id_subElf::MOUNTAIN_ELF);
            else if(subRaca == 3) elfo.beRace(id_subElf::SNOW_ELF);
            else {
                std::cout << "  Sub-raça inválida, usando Elfo de Sangue.\n";
                elfo.beRace(id_subElf::BOOD_ELF);   
            }

            globalRaceStats = elfo.getRaceStats();

            break;
        }

        case 3: {
            Dwarf anao;
            std::cout << "  Qual sub-raça de anão você quer?\n";
            std::cout << "  1. Anão Ferreiro (bônus em VIG e DEF)\n";
            std::cout << "  2. Anão das Colinas (bônus em DEF e FOR)\n";
            std::cout << "  3. Anão do Subsolo (bônus em VIG e FOR)\n";
            std::cout << "  > ";
            int subRaca;
            std::cin >> subRaca;
            if(subRaca == 1) anao.beRace(id_subDwarf::FORGE_DWARF);
            else if(subRaca == 2) anao.beRace(id_subDwarf::COULINS_DWARF);
            else if(subRaca == 3) anao.beRace(id_subDwarf::UNDERGROUND_DWARF);
            else {
                std::cout << "  Sub-raça inválida, usando Anão Ferreiro.\n";
                anao.beRace(id_subDwarf::FORGE_DWARF);
            }
            globalRaceStats = anao.getRaceStats();
            break;
        }
    }

    /* escolha da classe */
    int classe = escolherClasse();
    
    /* inicia a arena com a classe escolhida */
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
            std::cout << "Classe invalida — iniciando como Guerreiro.\n";
            Warrior guerreiro;
            arena(guerreiro, nome);
            break;
        }
    }

    return 0;
}