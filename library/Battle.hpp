#pragma once
#include "library/Stats/HP.hpp"
#include "library/character/Moves/Moves.hpp"
#include "library/Stats/Mana.hpp"
#include "library/Stats/Effects.hpp"
#include <iostream>
#include <string>

using std::cout;
using std::cin;

/* ═══════════════════════════════════════════════════
   Struct de inimigo — dados completos
═══════════════════════════════════════════════════ */

struct EnemyData {
    std::string nome;
    int         hp;
    uint32_t    damage;
    uint32_t    shield;
    uint32_t    expRecompensa;
    uint32_t    coinRecompensa;

};

/* ═══════════════════════════════════════════════════
   Arena progressiva — igual ao código C original
═══════════════════════════════════════════════════ */

inline std::vector<EnemyData> arenaInimigos = {
    { "Goblin",           14,  8,  0,  40,  10  },
    { "Goblin Arqueiro",  10,  8,  0,  44,  12  },
    { "Goblin Guerreiro", 14,  9,  1,  44,  12  },
    { "Goblin de Elite",  18,  10,  1,  55,  18  },
    { "Lobo Selvagem",    28,  10,  2,  60,  25  },
    { "Lobo Alfa",        32,  12,  2,  78,  35  },
    { "Javali Selvagem",  45,  12,  3,  98,  50  },
    { "Javali Selvagem",  45,  12,  3,  98,  50  },
    { "Javali Selvagem",  45,  15,  3,  98,  50  },
    { "Ogro",             45,  17,  3,  95,  50  },
    { "Ogro",             45,  17,  3,  95,  50  },
    { "Ogro Chefe",       60,  20,  4, 150,  80  },
    { "Dragao",           80,  25,  5, 200, 150  }
};

/* ═══════════════════════════════════════════════════
   Barra de HP visual
═══════════════════════════════════════════════════ */

inline void printBarraHP(const std::string &nome, int current, int max) {
    int total  = 30;
    int cheios = (max > 0) ? (current * total / max) : 0;

    cout << "  " << nome << "\n   [";
    for (int i = 0; i < total; i++)
        cout << (i < cheios ? "\33[31m❤\33[0m " : "▪ ");
    cout << "] " << current << "/" << max << std::endl;
}

inline void printBarraMana( int current, int max) {
    int total  = 30;
    int cheios = (max > 0) ? (current * total / max) : 0;

    cout << "   [";
    for (int i = 0; i < total; i++)
        cout << (i < cheios ? "\33[34m🔹\33[0m " : "▪ ");
    cout << "] " << current << "/" << max << std::endl;
}

/* ═══════════════════════════════════════════════════
   Batalha principal — integra moves + HP + crítico
═════════════════════════════════════════════════ */

template <typename T>
bool battle(T &jogador, EnemyData &inimigo, const std::string &nomeJogador) {

    int hpInimigo    = inimigo.hp;
    int hpInimigoMax = inimigo.hp;
    int turno        = 1;

    cout << "\n========================================\n";
    cout << "  BATALHA: " << nomeJogador << " vs " << inimigo.nome << "\n";
    cout << "========================================\n";

    while (jogador.isAlive() && hpInimigo > 0) {

        cout << "\n  -- Turno: " << turno << " --\n";
        printBarraHP(nomeJogador,  jogador.getHP(), jogador.getMaxHP());
        printBarraMana( jogador.getCurrentMana(), jogador.getMaxMana());
        printBarraHP(inimigo.nome, hpInimigo,       hpInimigoMax);
        
        /* turno do jogador — escolhe move */
        const MoveData *move = jogador.escolherMove(jogador.getNivel());

        if (move) {
            /* cura (para Cleric) */
            if (move->curaBase > 0) {
                jogador.getHeal(move->curaBase);
                cout << "  > " << move->nome
                     << " — curou " << move->curaBase << " HP!\n";

                jogador.useMana(move->costMana);
                cout << "  > Mana restante: " << jogador.getCurrentMana() << "\n";
            }

            /* dano no inimigo */
            if (move->danoBase > 0) {
                /* aplica stats do jogador no dano */
                finalStats fs = jogador.getStats();
                jogador.CriticalDamage(fs);
                int danoFinal = move->danoBase + (int)fs.Damage;
                int danoComShield = danoFinal - (int)(inimigo.shield);
                if (danoComShield < 0) danoComShield = 0;

                hpInimigo -= danoComShield;
                if (hpInimigo < 0) hpInimigo = 0;

                cout << "  > " << move->nome
                     << " causou " << danoComShield
                     << " de dano em " << inimigo.nome << "!\n";

                jogador.useMana(move->costMana);
                cout << "  > Mana restante: " << jogador.getCurrentMana() << "\n";
            }
        }

        if (hpInimigo <= 0) break;

        /* turno do inimigo */
        finalStats iniStats;
        iniStats.Damage = inimigo.damage;
        iniStats.Shield = inimigo.shield;
        iniStats.MaxHP  = inimigo.hp;
        iniStats.Speed  = 0;
        iniStats.MaxMana = 0;

        cout << "  > " << inimigo.nome << " ataca!\n";
        jogador.takeDamage(iniStats);

        turno++;
    }

    cout << "\n";
    printBarraHP(nomeJogador, jogador.getHP(), jogador.getMaxHP());
    cout << "\n";

    if (hpInimigo <= 0) {
        cout << "  Voce derrotou " << inimigo.nome << "!\n";
        cout << "  + " << inimigo.expRecompensa  << " EXP\n";
        cout << "  + " << inimigo.coinRecompensa << " moedas\n";
        return true;
    }

    cout << "  Voce foi derrotado por " << inimigo.nome << ". Game Over.\n";
    return false;
}

/* ═══════════════════════════════════════════════════
   Loop da arena — percorre todos os inimigos
═══════════════════════════════════════════════════ */

template <typename T>
void arena(T &jogador, const std::string &nomeJogador) {
    finalStats fs = jogador.getStats(); // Ou jogador.hp.getStats() se mudar a arquitetura
    int crtRate = 0; // Ou o valor vindo do sistema de status do seu jogador
    for (auto &inimigo : arenaInimigos) {

        cout << "\nProximo desafio: " << inimigo.nome
             << " (HP: " << inimigo.hp
             << " | Dano: " << inimigo.damage
             << " | Shield: " << inimigo.shield << ")\n";

        cout << "O que deseja fazer?\n";
        cout << "  1. Entrar em batalha\n";
        cout << "  2. Usar Consumíveis\n";
        cout << "> ";

        int esc;
        cin >> esc;

        if (esc == 2) {
            jogador.exibirinventory();
            cout << "Digite o numero do item que deseja usar (1-" << jogador.inventory.size() << "): ";
            int itemOp;
            cin >> itemOp;

            if (itemOp >= 1 && itemOp <= static_cast<int>(jogador.inventory.size())) {
                ConsumableID itemId = static_cast<ConsumableID>(itemOp);
                jogador.usarConsumivel(itemId, fs, crtRate);
            } else {
                cout << "  Opção inválida!\n";
            }
        }

        if (!battle(jogador, inimigo, nomeJogador))
            return;

        /* level up após batalha */
        jogador.gainExp(inimigo.expRecompensa);

        cout << "========================================\n";
        cout << "  " << nomeJogador
             << " | Nivel " << jogador.getNivel()
             << " | HP " << jogador.getHP()
             << "/" << jogador.getMaxHP() << "\n";
        cout << "========================================\n";
    }

    cout << "\n========================================\n";
    cout << "  " << nomeJogador << " completou a arena!\n";
    cout << "========================================\n";
}