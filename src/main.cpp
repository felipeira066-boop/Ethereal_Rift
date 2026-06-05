#include <iostream>
#include <windows.h>
#include <locale>
#include <string>
#include "bibliotecas.hpp"

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
    std::cout << "  ESCOLHA SUA CLASSE\n\n";
    std::cout << "  [1] GUERREIRO        • Defesa e Força altas - Ideal para combate direto\n";
    std::cout << "  [2] CAÇADOR          • Velocidade e Força medianas - Ataques rápidos\n";
    std::cout << "  [3] MAGO             • Inteligência alta - Magia e dano de longo alcance\n";
    std::cout << "  [4] CLÉRIGO          • Vigor alto - Pode se curar e ajudar aliados\n";
    printSep();
    std::cout << "  Qual é sua escolha, Aventureiro? > ";

    int op;
    std::cin >> op;
    return op;
}   

int escolherRaca(){
    printSep();
    std::cout << "  ESCOLHA SUA RAÇA \n\n";
    std::cout << "  [1] HUMANO   • Versáteis e equilibrados - Bom para iniciantes\n";
    std::cout << "  [2] ELFO     • Ágeis e mágicos - Velocidade e inteligência\n";
    std::cout << "  [3] ANÃO     • Fortes e resistentes - Defesa e vigor\n";
    printSep();
    std::cout << "  Qual raça você escolhe? > ";

    int op;
    std::cin >> op;
    return op;
}

/* ═══════════════════════════════════════════════════
   Main
═══════════════════════════════════════════════════ */

int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul");

    std::setlocale(LC_ALL, ".UTF-8");

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
    std::cout << "\n  Qual é seu nome, viajante? > ";
    std::getline(std::cin, nome);
    if (nome.empty()) nome = "Herói";

    /* Escolha das Raças */
    int raca = escolherRaca();

    switch(raca){
        case 1: {
            Human humano;
            std::cout << "\n  Escolha sua linhagem de Humano:\n\n";
            std::cout << "  [1]  Humano da Selva      • Equilibrado com bônus em Força\n";
            std::cout << "  [2]  Humano das Montanhas • Equilibrado com bônus em Vigor\n";
            std::cout << "  > ";
            int subRaca;
            std::cin >> subRaca;
            if(subRaca == 1) {
                humano.beRace(id_subHuman::JUNGLE_HUMAN);
                std::cout << "   Você é um Humano da Selva! Prepare-se para as aventuras...\n";
            }
            else if(subRaca == 2) {
                humano.beRace(id_subHuman::MOUNTAIN_HUMAN);
                std::cout << "   Você é um Humano das Montanhas! Sua resistência é incomparável.\n";
            }
            else {
                std::cout << "   Escolha inválida! Assumindo Humano da Selva.\n";
                humano.beRace(id_subHuman::JUNGLE_HUMAN);
            }

            globalRaceStats = humano.getRaceStats();
            break;
        }

        case 2:{
            Elf elfo;
            std::cout << "\n  Escolha sua linhagem de Elfo:\n\n";
            std::cout << "  [1]  Elfo de Sangue       • Vigor e Inteligência elevados\n";
            std::cout << "  [2]  Elfo das Montanhas   • Velocidade e Força superiores\n";
            std::cout << "  [3]  Elfo das Neves       • Resistência e Velocidade aprimoradas\n";
            std::cout << "  > ";
            int subRaca;
            std::cin >> subRaca;
            if(subRaca == 1) {
                elfo.beRace(id_subElf::BOOD_ELF);
                std::cout << "   Você é um Elfo de Sangue! Poder ancestral flui por suas veias.\n";
            }
            else if(subRaca == 2) {
                elfo.beRace(id_subElf::MOUNTAIN_ELF);
                std::cout << "   Você é um Elfo das Montanhas! A velocidade é sua aliada.\n";
            }
            else if(subRaca == 3) {
                elfo.beRace(id_subElf::SNOW_ELF);
                std::cout << "   Você é um Elfo das Neves! Frio e graça definem você.\n";
            }
            else {
                std::cout << "   Escolha inválida! Assumindo Elfo de Sangue.\n";
                elfo.beRace(id_subElf::BOOD_ELF);   
            }

            globalRaceStats = elfo.getRaceStats();

            break;
        }

        case 3: {
            Dwarf anao;
            std::cout << "\n  Escolha sua linhagem de Anão:\n\n";
            std::cout << "  [1]  Anão Ferreiro       • Vigor e Defesa superiores\n";
            std::cout << "  [2]  Anão das Colinas    • Defesa e Força aumentadas\n";
            std::cout << "  [3]  Anão do Subsolo     • Vigor e Força das profundezas\n";
            std::cout << "  > ";
            int subRaca;
            std::cin >> subRaca;
            if(subRaca == 1) {
                anao.beRace(id_subDwarf::FORGE_DWARF);
                std::cout << "   Você é um Anão Ferreiro! Seus músculos podem quebrar montanhas.\n";
            }
            else if(subRaca == 2) {
                anao.beRace(id_subDwarf::COULINS_DWARF);
                std::cout << "   Você é um Anão das Colinas! Defesa inabalável é sua marca.\n";
            }
            else if(subRaca == 3) {
                anao.beRace(id_subDwarf::UNDERGROUND_DWARF);
                std::cout << "   Você é um Anão do Subsolo! Poder primordial corre em suas veias.\n";
            }
            else {
                std::cout << "   Escolha inválida! Assumindo Anão Ferreiro.\n";
                anao.beRace(id_subDwarf::FORGE_DWARF);
            }
            globalRaceStats = anao.getRaceStats();
            break;
        }

        default:
            std::cout << ("  ! Digito Errado ! \n");
            return 1;
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
            std::cout << "   Classe inválida! Você despertará como um Guerreiro...\n";
            Warrior guerreiro;
            arena(guerreiro, nome);
            break;
        }
    }

    

    return 0;
}